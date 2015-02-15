/*
 *author:yueruipeng@letv.com
 *date:2014/12/31
 *file:hdfswriter.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_HDFSWRITER_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_HDFSWRITER_HPP_

#include "mutex.hpp"
#include "lock.hpp"
#include "threadif.hpp"
#include "loader.hpp"

//дhdfs
class HdfsWriter : public ThreadIf
{
public:
  HdfsWriter(Loader& _owner);
  ~HdfsWriter();

  virtual void thread();

private:
  Loader& m_owner;
  unsigned int m_id;
  unsigned int m_count;
  unsigned int m_failCount;
};

#endif
