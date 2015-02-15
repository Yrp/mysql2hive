/*
 *author:yueruipeng@letv.com
 *date:2015/1/13
 *file:fileutil.cpp
 *
 */

#include "fileutil.hpp"
#include "logger.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


FileUtil::FileUtil(const std::string& _path)
  :m_path(_path)
{
}

FileUtil::FileUtil(const std::string& _dir, const std::string& _name)
  :m_name(_name), m_dir(_dir)
{
  m_path = m_dir + std::string("/") + m_name;
}

bool FileUtil::lockWrite(const std::string& _content, size_t _size) const
{
  int fd = ::open(m_path.c_str(), O_CREAT|O_WRONLY|O_APPEND, 0664);
  if (fd < 0)
  {
    LOG_ERR << "Open failed.(" << m_path << ")";
    return false;
  }

  // Add execulsive write lock on this file descriptor
  {
    struct flock writeLock;
    writeLock.l_type = F_WRLCK;
    writeLock.l_start = 0;
    writeLock.l_whence = SEEK_SET;
    writeLock.l_len = 0;
    writeLock.l_pid = -1;
    if (::fcntl(fd, F_SETLKW, &writeLock) < 0)
    {
      LOG_ERR << "Lock failed.(" << m_path << ")";
      ::close(fd);
      return false;
    }
  }

  // Write content to file
  bool ret = true;
  ssize_t sz, nReadBytes = _size, nWriteBytes = 0;
  const char *buffer = _content.c_str();
  do
  {
    if ((sz = ::write(fd, buffer + nWriteBytes, nReadBytes - nWriteBytes))< 0)
    {
    LOG_ERR << "Write failed.(" << m_path << ")";
      nReadBytes = sz;
      break;
    }
    nWriteBytes += sz;
  }
  while (nWriteBytes < nReadBytes);

  // Release the write lock
  {
    struct flock writeLock;
    writeLock.l_type = F_UNLCK;
    writeLock.l_start = 0;
    writeLock.l_whence = SEEK_SET;
    writeLock.l_len = 0;
    writeLock.l_pid = -1;
    if (::fcntl(fd, F_SETLKW, &writeLock) < 0)
    {
      ret = false;
      LOG_ERR << "Unlock failed.(" << m_path << ")";
    }
  }

  ::close(fd);
  return ret;
}

bool FileUtil::lockWrite(const std::string& _content) const
{
  return lockWrite(_content, _content.size());
}

bool FileUtil::read(std::string& _content) const
{
  if (!isRegular())
  {
    return false;
  }

  FILE* fp = fopen(m_path.c_str(), "r");
  if (!fp)
  {
    LOG_ERR << "Open failed.(" << m_path << ")";
    return false;
  }

  char buffer[10240];
  unsigned int nReadBytes;
  _content.reserve(size());
  while ((nReadBytes = fread(buffer, sizeof(char), 10240, fp)) > 0)
  {
    _content.append(buffer, nReadBytes);
  }

  if (ferror(fp))
  {
    LOG_ERR << "Read failed.(" << m_path << ")";
    fclose(fp);
    return false;
  }

  fclose(fp);
  return true;
}

bool FileUtil::rename(const std::string& _newPath)
{
  if (!isExists() || !isReadable())
  {
    return false;
  }

  if (::rename(m_path.c_str(), _newPath.c_str()) < 0)
  {
    //LOG_ERR << " m_path="<< m_path << " tofilePath=" << tofilePath;//test
    return false;
  }

  m_path = _newPath;

  return true;

}

bool FileUtil::isExists() const
{
  if (::access(m_path.c_str(), F_OK) != 0)
  {
    LOG_ERR << "file is not exists.(" << m_path << ")";
    return false;
  }

  return true;

}
bool FileUtil::isReadable() const
{
  if (!isExists())
  {
    return false;
  }
  else if (::access(m_path.c_str(), R_OK) != 0)
  {
    LOG_ERR << "file is not readable.(" << m_path << ")";
    return false;
  }

  return true;
}

bool FileUtil::isRegular() const
{
  struct stat statBuf;
  if (::stat(m_path.c_str(), &statBuf) != 0)
  {
    LOG_ERR << "Stat failed.(" << m_path << ")";
    return false;
  }
  else if (!S_ISREG(statBuf.st_mode))
  {
    LOG_ERR << "Is not a regular file.(" << m_path << ")";
    return false;
  }

  return true;
}

long FileUtil::size() const
{
  struct stat statBuf;
  if (stat(m_path.c_str(), &statBuf) < 0)
  {
    LOG_ERR << "Stat failed.(" << m_path << ")";
    return -1;
  }
  else if(!S_ISREG(statBuf.st_mode))
  {
    LOG_ERR << "Is not a regular file.(" << m_path << ")";
    return -1;
  }

  return statBuf.st_size;
}


