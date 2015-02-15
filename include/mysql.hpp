/*
 *author:yueruipeng@letv.com
 *date:2014/12/25
 *file:mysql.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_MYSQL_H_
#define COM_LETV_BIGDATA_MYSQL2HDFS_MYSQL_H_

#include <string>
#include <sstream>
#include "mysql.h"
#include "my_global.h"

class MySql
{
public:
  MySql(const std::string& _host, const std::string& _port, const std::string& _user,
        const std::string& _passwd, const std::string& _db);
  MySql();
  ~MySql();

  void init();
  MYSQL* connect(const std::string& _host, const std::string& _port, const std::string& _user,
            const std::string& _passwd, const std::string& _db);
  const MYSQL* getConnect() const { return &m_connect; }
  MYSQL_RES* query(const std::string& _query);

  std::string error();
  void close();

  inline const MYSQL_ROW nextRow() const { return mysql_fetch_row(m_result); }
  const int getRowNum() const { return m_rowNum; }
  const int getFieldNum() const { return m_fieldNum; }

private:
  MYSQL m_connect;
  MYSQL_RES* m_result;

  int m_rowNum;
  int m_fieldNum;
};

#endif
