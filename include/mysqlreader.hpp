/*
 *author:yueruipeng@letv.com
 *date:2014/12/31
 *file:mysqlreader.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_MYSQLREADER_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_MYSQLREADER_HPP_

#include "mutex.hpp"
#include "lock.hpp"
#include "threadif.hpp"
#include "loader.hpp"

//´Ómysql¶Á¼ÇÂ¼
class MysqlReader : public ThreadIf
{
public:
  MysqlReader(Loader& _owner);
  ~MysqlReader();

  virtual void thread();

private:
  Loader& m_owner;
  unsigned int m_id;
  unsigned int m_count;

};

#endif
