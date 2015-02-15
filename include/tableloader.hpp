/*
 *author:yueruipeng@letv.com
 *date:2015/1/4
 *file:tableloader.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_TABLELOADER_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_TABLELOADER_HPP_

#include "lock.hpp"
#include "mutex.hpp"
#include "fifo.hpp"
#include "table.hpp"

//获取需要处理的表
class TableLoader
{
public:
  ~TableLoader();

  void init();
  static TableLoader& Instance() { static TableLoader tableloader; return tableloader; }

  Table* getTable() { 
    Lock lock(m_mutex); 
    if(m_fifo.empty()) return NULL; 
    Table* tablePtr = m_fifo.front(); 
    m_fifo.pop_front(); 
    return tablePtr; 
  }

  void clear();
  
private:
  TableLoader() {};
  TableLoader(const TableLoader&);
  TableLoader& operator=(const TableLoader&);

  std::deque<Table*> m_fifo;
  mutable Mutex m_mutex;

};

#endif