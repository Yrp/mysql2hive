/*
 *author:yueruipeng@letv.com
 *date:2015/1/13
 *file:config.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_CONFIG_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_CONFIG_HPP_

#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

namespace ptree=boost::property_tree;
namespace json=boost::property_tree::json_parser;

class Config
{
public:
  ~Config();
  
  static Config& Instance() { static Config config; return config; }
  
  void load(const std::string& _path);
  
  const std::string getProp(const std::string& _prop, const std::string& _default=std::string()) const;
  const bool getProp(const std::string& _prop, bool _default=false) const;
  const int getProp(const std::string& _prop, const int _default=0) const;
private:
  Config();
  Config(const Config&);
  Config& operator=(const Config&); 
  
  ptree::ptree m_ptree;
  
};

#endif

