/*
 *author:yueruipeng@letv.com
 *date:2015/1/4
 *file:loader.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_LOADER_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_LOADER_HPP_

#include <string>
#include "fifo.hpp"
#include "mutex.hpp"
#include "threadif.hpp"
#include "lock.hpp"
#include "tablepair.hpp"

//负责读写线程的并发及消息队列的维护
class Loader : public ThreadIf
{
public:
  Loader(unsigned int _id, const int _fifoSize);
  ~Loader();

  virtual void thread();

  const unsigned int id() const { return m_id; }
  void setReadFinish(bool value) { Lock lock(m_mutex); m_isReadFinish = value; }
  void setWriteFinish(bool value) { Lock lock(m_mutex); m_isWriteFinish = value; }
  const bool isReadFinish() const { Lock lock(m_mutex); return m_isReadFinish; }
  const bool isWriteFinish() const { Lock lock(m_mutex); return m_isWriteFinish; }
  Fifo<TablePair>& getFifo() { return m_fifo; }
private:
  Fifo<TablePair> m_fifo;
  unsigned int m_id;
  bool m_isReadFinish;
  bool m_isWriteFinish;
  mutable Mutex m_mutex;

};

#endif