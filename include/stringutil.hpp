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

//�ַ�������������غ����������������������

//��by�����е��ַ��ָ��ַ���
int split_first_of(const std::string& in, std::vector<std::string>& out, const std::string& by);
int split(const std::string& in, std::vector<std::string>& out, const std::string& by);
int split(const std::string& in, std::set<std::string>& out, const std::string& by);


#endif

