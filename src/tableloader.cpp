/*
 *author:yueruipeng@letv.com
 *date:2015/1/4
 *file:tableloader.cpp
 *
 */

#include "tableloader.hpp"
#include "table.hpp"
#include "stringutil.hpp"
#include "logger.hpp"
#include "mysql.hpp"
#include "config.hpp"
#include "fileutil.hpp"
#include <stdlib.h>
#include <vector>
#include <set>

void TableLoader::init()
{
  LOG_INFO << "Initialize tableloader...";
  
  TableLoader::clear();
  
  //读取done,目前为止完成的所有表
  const std::string donePath = Config::Instance().getProp("done_path", std::string("./.done"));
  std::string doneStr;
  std::set<std::string> doneSet;
  bool doneRet = FileUtil(donePath).read(doneStr);
  if(doneRet)
  {
    split(doneStr, doneSet, "\n");
  }
    
  //读取undone，即上一次程序异常退出未完成的表
  const std::string undonePath = Config::Instance().getProp("undone_path",std::string("./undone"));
  const std::string query = Config::Instance().getProp("query",std::string(""));
  if (query.empty())
  {
    return;
  }
  
  std::string undoneStr;
  std::map<std::string, std::vector<std::string> > undoneMap;
  bool undoneRet = FileUtil(undonePath).read(undoneStr);
  if (undoneRet)
  {
    std::vector<std::string> undoneVec;
    split(undoneStr, undoneVec, "\n");
    for (int i=0; i<undoneVec.size(); ++i)
    {
      std::vector<std::string> vecTmp;
      split(undoneVec[i], vecTmp, "@");
      undoneMap[vecTmp[0]] = vecTmp;
    }
    
    std::map<std::string, std::vector<std::string> >::iterator it;
    for (it = undoneMap.begin(); it != undoneMap.end();)
    {
      if (atoi(it->second[1].c_str()) + atoi(it->second[2].c_str()) == atoi(it->second[3].c_str()))
      {
        undoneMap.erase(it++);
      }
      else
      {
        it++;
      }
    }
  }
  
  MySql mysql;
  if( mysql.connect(
    "xxx.xxx.xxx.xxx",/*ip*/
    "xxxx",/*port*/
    "xx",/*user*/
    "xx",/*passwd*/
    "xx"/*db*/
    ) == NULL )
  {
    LOG_ERR << "Failed to connect to database.(" << "xx" << ")";
    return;
  }

  const std::string sqlStrCmd = query;
    
  if(mysql.query(sqlStrCmd) == NULL)
  {
    return;
  }
  MYSQL_ROW row;
  while ((row = mysql.nextRow()))
  {
/*test***/
    if (std::string(row[0]).find("10.150.140.111") == std::string::npos)
    {
      continue;
    }
//    if (std::string(row[1]).find("base_bw201501072") == std::string::npos)
//    {
//      continue;
//    }
/*test***/
    std::vector<std::string> vecDb;
    if ( split_first_of(row[0], vecDb, "/@:") != 5 )
    {
      continue;
    }

    Table* tablePtr = new Table;
    tablePtr->setUser(vecDb[0]);
    tablePtr->setPasswd(vecDb[1]);
    tablePtr->setHost(vecDb[2]);
    tablePtr->setPort(vecDb[3]);
    tablePtr->setDb(vecDb[4]);
    tablePtr->setTable(row[1]);
    
    if (doneRet && doneSet.find(tablePtr->name()) != doneSet.end())
    {
      delete tablePtr;
      continue;
    }
    
    std::map<std::string, std::vector<std::string> >::iterator
      it = undoneMap.find(tablePtr->name());
    if (undoneRet && it != undoneMap.end())
    {
      tablePtr->setEx();
      undoneMap.erase(it);
    }
    
    m_fifo.push_back(tablePtr);
  }
  
  if (undoneRet && !undoneMap.empty())//从mysql读取的表中没有该异常表，则加入待读表单
  {
    std::map<std::string, std::vector<std::string> >::iterator it;
    for (it = undoneMap.begin(); it != undoneMap.end(); ++it)
    {
      std::vector<std::string> tableVec;
      split(it->first, tableVec, "-");
      
      Table* tablePtr = new Table;
      tablePtr->setHost(tableVec[0]);
      tablePtr->setPort(tableVec[1]);
      tablePtr->setUser(tableVec[2]);
      tablePtr->setPasswd(tableVec[3]);
      tablePtr->setDb(tableVec[4]);
      tablePtr->setTable(tableVec[5]);
      tablePtr->setEx();
      
      m_fifo.push_back(tablePtr);
    }
  }

  std::string tables;
  std::deque<Table*>::iterator it;
  for ( it=m_fifo.begin(); it != m_fifo.end(); ++it)
  {
    tables += (*it)->str() + "\n";
  }
  LOG_INFO << "The following tables will be  handled(amount:" << m_fifo.size() << ").\n" << tables; 


  std::stringstream ss;
  ss << time(0);
  if (!FileUtil(undonePath).rename(undonePath + "." + ss.str()))
  {
    LOG_ERR << "Failed to rename (oldPath="<< undonePath << ", newPath=" << undonePath + "." + ss.str() << ")";
  }

  LOG_INFO << "Succeeded to initialize tableloader.";

}

void TableLoader::clear()
{
  while(!m_fifo.empty())
  {
    Table* tablePtr = m_fifo.front();
    m_fifo.pop_front();
    delete tablePtr;
  }
  m_fifo.clear();
}

TableLoader::~TableLoader()
{
  Lock lock(m_mutex);
  while(!m_fifo.empty())
  {
    Table* tablePtr = m_fifo.front();
    m_fifo.pop_front();
    delete tablePtr;
  }
}
