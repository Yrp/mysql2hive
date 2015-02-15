/*
 *author:yueruipeng@letv.com
 *date:2014/12/25
 *file:tablepair.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_TABLEFIFO_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_TABLEFIFO_HPP_

#include "fifo.hpp"
#include "mutex.hpp"
#include "lock.hpp"
#include <string>
#include <sstream>

class TablePair
{
public:

  TablePair() :m_offset(0), m_size(0), m_total(0), m_ex(false) {};
  ~TablePair(){};

  const std::string& getTable() const { return m_table; }
  const std::string& getRecords() const { return m_records; }
  const unsigned int getOffset() const { return m_offset ; }
  const unsigned int getSize() const { return m_size; }
  const bool getEx() const { return m_ex; }
  const unsigned int getTotal() const { return m_total; }

  void setOffset(unsigned int _offset) { m_offset = _offset; }
  void setSize(unsigned int _size) { m_size = _size; }
  void setTable(const std::string& _table) { m_table = _table; }
  void append(const std::string& _record) { m_records.append(_record); }
  void setTotal(unsigned int _total) { m_total = _total; }
  void setEx() { m_ex = !m_ex; }
  
  const std::string info() const { 
    std::stringstream ss;
    ss << m_table << "@" << m_offset << "@" << m_size << "@" << m_total;
    return ss.str(); }
    
private:
  unsigned int m_offset;
  unsigned int m_size;
  unsigned int m_total;
  bool m_ex;
  std::string m_table;
  std::string m_records;
};

#endif
