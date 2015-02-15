/*
 *author:yueruipeng@letv.com
 *date:2015/1/6
 *file:hdfs.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_HDFS_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_HDFS_HPP_

#include "hdfs.h"
#include <string>


class Hdfs
{
public:
  Hdfs();
  Hdfs(const std::string& _nn, tPort _port);
  ~Hdfs();

  bool connect(const std::string& _nn="default", tPort _port=0);
  bool open(const std::string& _path, int _flags=O_WRONLY|O_CREAT, int _bufferSize=0, short _replication=0, tSize _blockSize=0);
  tSize write(const std::string& _buffer, tSize _length);
  tSize write(const std::string& _buffer);
  tSize write(const void* _buffer, tSize _length);

  bool exist(const std::string& _path);
  bool flush();
  bool hflush();
  bool hsync();
  bool close();

private:
  static const unsigned int BUFFER_MAX_SIZE = 10 * 1024 * 1024;
  hdfsFS m_fs;
  hdfsFile m_write;

};

#endif
