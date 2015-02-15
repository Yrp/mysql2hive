/*
 *author:yueruipeng@letv.com
 *date:2014/12/25
 *file:table.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_TABLE_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_TABLE_HPP_

#include <string>
#include <vector>

//待处理的表信息
class Table
{
public:
  Table() :m_ex(false) {};
  ~Table() {};

  const std::string& getHost() const { return m_host; }
  const std::string& getPort() const { return m_port; }
  const std::string& getUser() const { return m_user; }
  const std::string& getPasswd() const { return m_passwd; }
  const std::string& getDb() const { return m_db; }
  const std::string& getTable() const { return m_table; }
  bool getEx() const { return m_ex; }

  void setHost(const std::string& _host) { m_host = _host; }
  void setPort(const std::string& _port) { m_port = _port; }
  void setUser(const std::string& _user) { m_user = _user; }
  void setPasswd(const std::string& _passwd) { m_passwd = _passwd; }
  void setDb(const std::string& _db) { m_db = _db; }
  void setTable(const std::string& _table) { m_table = _table; }
  void setEx() { m_ex = !m_ex; }
  
  const std::string str() const { return m_host + "|" + m_port + "|" + m_user + "|" + m_passwd + "|" + m_db + "|" + m_table; }
  const std::string name() const { return m_host + "-" + m_port + "-" + m_user + "-" + m_passwd + "-" + m_db + "-" + m_table; }

private:
  bool m_ex;//上一次程序异常
  std::string m_host;
  std::string m_port;
  std::string m_user;
  std::string m_passwd;
  std::string m_db;
  std::string m_table;
};

#endif