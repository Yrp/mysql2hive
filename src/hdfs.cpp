/*
 *author:yueruipeng@letv.com
 *date:2015/1/6
 *file:hdfs.cpp
 *
 */

#include "hdfs.hpp"
#include <iostream>
Hdfs::Hdfs()
  :m_fs(NULL), m_write(NULL)
{
}

Hdfs::Hdfs(const std::string& _nn, tPort _port)
  :m_fs(NULL), m_write(NULL)
{
  connect(_nn, _port);
}

Hdfs::~Hdfs()
{
//   close();
}

bool Hdfs::connect(const std::string& _nn, tPort _port)
{
  m_fs = hdfsConnect(_nn.c_str(), _port);
  return m_fs != NULL;
}

bool Hdfs::open(const std::string& _path, int _flags, int _bufferSize, short _replication, tSize _blockSize)
{
  m_write = hdfsOpenFile(m_fs, _path.c_str(), _flags, _bufferSize, _replication, _blockSize);
  return m_write != NULL;
}

tSize Hdfs::write(const std::string& _buffer, tSize _length)
{
  return write((void*)(_buffer.c_str()), _buffer.size());
}

tSize Hdfs::write(const std::string& _buffer)
{
  return write(_buffer, _buffer.size());
}

tSize Hdfs::write(const void* _buffer, tSize _length)
{
  tSize written = hdfsWrite(m_fs, m_write, _buffer, _length);
  return written;
}

bool Hdfs::exist(const std::string& _path)
{
  return hdfsExists(m_fs, _path.c_str())==0;
}

bool Hdfs::flush()
{
  return hdfsFlush(m_fs, m_write)==0;
}

bool Hdfs::hflush()
{
  return hdfsHFlush(m_fs, m_write)==0;
}

bool Hdfs::hsync()
{
  return hdfsHSync(m_fs, m_write)==0;
}

bool Hdfs::close()
{
  return hflush() & (hdfsCloseFile(m_fs, m_write)==0);
}
