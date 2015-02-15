/*
 *author:yueruipeng@letv.com
 *date:2014/12/31
 *file:loader.cpp
 *
 */

#include "loader.hpp"
#include "mysqlreader.hpp"
#include "hdfswriter.hpp"
#include "logger.hpp"

Loader::Loader(unsigned int _id, const int _fifoSize)
  :m_fifo(_fifoSize), m_id(_id)
{
}

Loader::~Loader()
{
}

void Loader::thread()
{
  LOG_INFO << "loader thread(" << m_id << ") is running...";
  size_t begin = time(0);

  //读线程
  MysqlReader mysqlReader(*this);
  setReadFinish(false);
  mysqlReader.run();

  //写线程
  HdfsWriter hdfsWriter(*this);
  setWriteFinish(false);
  hdfsWriter.run();

  while ( !mysqlReader.waitForShutdown(2000) ) {}
  setReadFinish(true);

  while ( !hdfsWriter.waitForShutdown(2000) ) {}
  setWriteFinish(true);

  size_t end = time(0);
  LOG_INFO << "loader thread(" << m_id << ") is finished."
           << " [used time: " << end-begin << "s.]";
  shutdown();
  return;

}

