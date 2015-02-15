/*
 *author:yueruipeng@letv.com
 *date:2014/12/25
 *file:mysql.cpp
 *
 */

#include "../include/mysql.hpp"
#include "../include/logger.hpp"

MySql::MySql()
  :m_result(NULL), m_rowNum(0), m_fieldNum(0)
{
  this->init();
}

MySql::MySql(const std::string& _host, const std::string& _port, const std::string& _user,
        const std::string& _passwd, const std::string& _db)
  :m_result(NULL), m_rowNum(0), m_fieldNum(0)
{
  this->init();
  this->connect(_host, _port, _user, _passwd, _db);
}

void MySql::init()
{
  LOG_INFO << "Initializing MySql connect...";
  if (mysql_library_init(0, NULL, NULL))
  {
    LOG_ERR << "Could not initialize MySql library." << this->error();
  }

  if (mysql_init(&m_connect) == NULL)
  {
    LOG_ERR << "Failed to initialize MySql connect." << this->error();
  }

  LOG_INFO << "Succeeded to initialize MySql connect.";
  mysql_options(&m_connect, MYSQL_SET_CHARSET_NAME, "utf8");
}

MYSQL* MySql::connect(const std::string& _host, const std::string& _port, const std::string& _user,
            const std::string& _passwd, const std::string& _db)
{
  if ( mysql_real_connect(&m_connect, _host.c_str(), _user.c_str(), _passwd.c_str(), 
    _db.c_str(), atoi(_port.c_str()), 0, CLIENT_MULTI_STATEMENTS) == NULL )
  {
    LOG_ERR << "Failed to connect to database.("
            << "host:" << _host
            << ",port:" << _port
            << ",user:" << _user
            << ",password:" << _passwd
            << ",db:" << _db
            << ".)"
            << this->error();
    return NULL;
  }

  LOG_INFO << "Succeeded to connect to database.("
           << "host:" << _host
           << ",port:" << _port
           << ",user:" << _user
           << ",password:" << _passwd
           << ",db:" << _db
           << ".)";
  return &m_connect;
}

std::string MySql::error()
{
  std::stringstream ss;
  ss << "[errno:" << mysql_errno(&m_connect) << ", errinfo:" << mysql_error(&m_connect) << "]";
  return ss.str();
}

void MySql::close()
{
  mysql_free_result(m_result);
  mysql_close(&m_connect);
  LOG_INFO << "Close connection.";
}

MYSQL_RES* MySql::query(const std::string& _query)
{
  if (m_result != NULL)
  {
    mysql_free_result(m_result);
    m_result = NULL;    
  }
  
  int ret = mysql_real_query(&m_connect, _query.c_str(), _query.size());
  if (ret != 0)
  {
    LOG_ERR << this->error() << "(" << _query << ")";
    return NULL;
  }

  LOG_INFO << "Query Succeeded.(" << _query << ")";

  m_result = mysql_use_result(&m_connect);
  if(m_result == NULL)
  {
    LOG_ERR << this->error() << "(" << _query << ")";
    return NULL;
  }
  m_fieldNum = mysql_num_fields(m_result);
  m_rowNum = mysql_num_rows(m_result);
  return m_result;
}

MySql::~MySql()
{
  this->close();
}