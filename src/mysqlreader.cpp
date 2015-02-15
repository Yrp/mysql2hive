/*
 *author:yueruipeng@letv.com
 *date:2014/12/31
 *file:mysqlreader.cpp
 *
 */

#include "mysqlreader.hpp"
#include "logger.hpp"
#include "mysql.hpp"
#include "fifo.hpp"
#include "table.hpp"
#include "tableloader.hpp"
#include "loader.hpp"
#include "config.hpp"
#include "fileutil.hpp"
#include "runcommand.hpp"
#include <string>


MysqlReader::MysqlReader(Loader& _owner)
  :m_owner(_owner), m_id(0), m_count(0)
{
}

MysqlReader::~MysqlReader()
{
}

void MysqlReader::thread()
{
  LOG_INFO << "loader thread(" << m_owner.id() << ") mysqlreader thread(" << m_id << ") is running...";

  const std::string donePath = Config::Instance().getProp("done_path", std::string("./.done"));
  const unsigned int batchSize = Config::Instance().getProp("batch_size", 100000);
  const std::string destDir = Config::Instance().getProp("dest_dir", std::string());
  Fifo<TablePair>& fifo = m_owner.getFifo();
  int count = 0;
  size_t begin = time(0);

  Table* tablePtr;
  while ( (tablePtr = TableLoader::Instance().getTable()) != NULL )
  {
    LOG_INFO << "reading table(" << tablePtr->str() << ")";
        
    size_t begin = time(0);
    MySql mysql;
    if (mysql.connect(
        tablePtr->getHost(),
        tablePtr->getPort(),
        tablePtr->getUser(),
        tablePtr->getPasswd(),
        tablePtr->getDb()
    ) == NULL)
    {
      LOG_ERR << "Failed to connect database.(" << tablePtr->str() << ")";
      continue;
    }
    
    const std::string totalSqlCmdStr = "select count(*) from " + tablePtr->getTable();
    if (mysql.query(totalSqlCmdStr) == NULL)
    {
      LOG_ERR << "failed to query.(" << totalSqlCmdStr << ")";
      continue;
    }
    int total = atoi(mysql.nextRow()[0]);

    const std::string sqlCmdStr = "select * from " + tablePtr->getTable();
    if (mysql.query(sqlCmdStr) == NULL)
    {
      LOG_ERR << "failed to query.(" << sqlCmdStr << ")";
      continue;
    }
    const int num_fields = mysql.getFieldNum();
    MYSQL_ROW row;
    const std::string delimiter = "|";
    unsigned int recordNum = 0;
    TablePair* tablePairPtr = new TablePair;
    tablePairPtr->setTable(tablePtr->name());
    tablePairPtr->setOffset(0);
    tablePairPtr->setTotal(total);
    if (tablePtr->getEx())//上一次处理是否异常
    {
      tablePairPtr->setEx();
    }
    while ((row = mysql.nextRow()))
    {
      recordNum++;
      std::string record;
      for (int i=0; i < num_fields; i++)
      {
        record.append(row[i]);
        record.append(delimiter);
      }
      record.append("\n");
//      std::cout << record;//test

      tablePairPtr->append(record);
      count++;
      
      if ( recordNum >= batchSize)
      {
        tablePairPtr->setSize(recordNum);
        fifo.add(tablePairPtr);
        tablePairPtr = new TablePair;
        tablePairPtr->setTable(tablePtr->name());
        tablePairPtr->setOffset(count);
        tablePairPtr->setTotal(total);
        recordNum = 0;
      }
    }
    m_count += count;
    tablePairPtr->setSize(recordNum);
    fifo.add(tablePairPtr);

    const std::string tableInfoStr = tablePtr->str();
    FileUtil(donePath).lockWrite(tablePtr->name()+"\n");
    size_t end = time(0);
    LOG_INFO << "end to read table(" << tableInfoStr << ") [record count:" << count << ", used time: " << end-begin << "s.]";
    count = 0;

    //在hive里建分区，使用system调用hive -e命令
    const std::string tableName = tablePtr->getTable();
    delete tablePtr;
    tablePtr = NULL;
    int pos = tableName.find("base_bw") + std::string("base_bw").size();
    const std::string pday = tableName.substr(pos, 8) ;
    const std::string phour = tableName.substr(pos+8, 2);
    const std::string commandStr ="hive -e \"use data_raw;alter table cdn_base_bw add partition(" \
      "pday=" + pday + ",phour=" + phour + ") location '" + destDir + "/pday=" + pday + "/phour=" + phour +"';\"";
    if (!runCommand(commandStr))
    {
      LOG_ERR << "failed to run command.Please check up and run command manually.[" << commandStr << "]";
    }
  
  }

  size_t end = time(0);
  LOG_INFO << "loader thread(" << m_owner.id() << ") mysqlreader thread(" << m_id << ") is finished."
           << " [record count:" << m_count << ", used time: " << end-begin << "s.]";
  shutdown();
  return;
}
