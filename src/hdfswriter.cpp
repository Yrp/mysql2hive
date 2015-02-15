/*
 *author:yueruipeng@letv.com
 *date:2014/12/31
 *file:hdfswriter.cpp
 *
 */

#include "hdfswriter.hpp"
#include "logger.hpp"
#include "fifo.hpp"
#include "loader.hpp"
#include "hdfs.hpp"
#include "config.hpp"
#include "fileutil.hpp"
#include <string>
#include <cstdlib>

HdfsWriter::HdfsWriter(Loader& _owner)
  :m_owner(_owner), m_id(0), m_count(0), m_failCount(0)
{
}

HdfsWriter::~HdfsWriter()
{
}

void HdfsWriter::thread()
{
  LOG_INFO << "loader thread(" << m_owner.id() << ") hdfswriter thread(" << m_id << ") is running...";
  
  const std::string undonePath = Config::Instance().getProp("undone_path",std::string("./undone"));
  const std::string destDir = Config::Instance().getProp("dest_dir", std::string());

  if (destDir.empty())
  {
    LOG_ERR << "output Directory is empty.";
    shutdown();
    return;
  }

  Hdfs hdfs;
  if(!hdfs.connect())
  {
    LOG_ERR << "Cannot connect to HDFS.";
    shutdown();
    return;
  }

  unsigned int begin = time(0);
  Fifo<TablePair>& fifo = m_owner.getFifo();
  while(true)
  {
    TablePair* record = fifo.getNext(500);
    if (record == NULL && m_owner.isReadFinish() )
    {
      break;
    }
    else if ( record == NULL )
    {
      m_failCount++;
      continue;
    }

    const std::string tableName = record->getTable();
    int pos = tableName.find("base_bw") + std::string("base_bw").size();
    const std::string path = destDir 
      + "/pday=" + tableName.substr(pos, 8) 
      + "/phour=" + tableName.substr(pos+8, 2) 
      + "/" + tableName;
    if (hdfs.exist(path))
    {
      if (!record->getEx()) //上一次处理表时是否异常退出
      {
        if ( !hdfs.open(path, O_WRONLY|O_APPEND) )
        {
          LOG_ERR << "Failed to open file for writing![file: " << path << "]";
          shutdown();
          return;
        }
      }
      else
      {
        if ( !hdfs.open(path, O_WRONLY|O_TRUNC) )
        {
          LOG_ERR << "Failed to open file for writing![file: " << path << "]";
          shutdown();
          return;
        }        
      }
    }
    else
    {
      if ( !hdfs.open(path, O_WRONLY|O_CREAT) )
      {
        LOG_ERR << "Failed to open file for writing![file: " << path << "]";
        shutdown();
        return;
      }
    }

    unsigned int written = hdfs.write(record->getRecords());
    hdfs.close();
    m_count += record->getSize();
    const std::string info = record->info();
    const unsigned int size = record->getSize();
    const unsigned int total = record->getTotal();
    delete record;
    FileUtil(undonePath).lockWrite(info + "\n");
    LOG_INFO << "written " << written << "B (record num: " << size << ") to " << path << "(total num: " << total << ")";
  }

  unsigned int end = time(0);
  LOG_INFO << "loader thread(" << m_owner.id() << ") hdfswriter thread(" << m_id << ") is finished."
           << " [record count:" << m_count << ", used time: " << end-begin << "s.]";
  shutdown();
  return;
}
