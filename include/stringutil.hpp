/*
 *author:yueruipeng@letv.com
 *date:2015/1/5
 *file:tableloader.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_STRINGUTIL_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_STRINGUTIL_HPP_

#include <string>
#include <vector>
#include <set>

//字符串处理所有相关函数，待添加完善其他函数

//以by中所有的字符分隔字符串
int split_first_of(const std::string& in, std::vector<std::string>& out, const std::string& by);
int split(const std::string& in, std::vector<std::string>& out, const std::string& by);
int split(const std::string& in, std::set<std::string>& out, const std::string& by);


#endif

