/*
 *author:yueruipeng@letv.com
 *date:2015/1/13
 *file:fileutil.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_FILEUTIL_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_FILEUTIL_HPP_

#include <string>

class FileUtil
{
public:
  FileUtil(const std::string& _path);
  FileUtil(const std::string& _dir, const std::string& _name);
    
  bool lockWrite(const std::string& _content, size_t _size) const;
  bool lockWrite(const std::string& _content) const;
    
  bool read(std::string& _content) const;

  bool rename(const std::string& _newPath);
  bool isExists() const;
  bool isReadable() const;

  bool isRegular() const;
  long size() const;

private:
  std::string m_name;
  std::string m_dir;
  std::string m_path;
};

#endif