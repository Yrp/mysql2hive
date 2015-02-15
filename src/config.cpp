/*
 *author:yueruipeng@letv.com
 *date:2015/1/13
 *file:config.cpp
 *
 */

#include "config.hpp"

Config::Config()
{
}

Config::~Config()
{
}

void Config::load(const std::string& _path)
{
  json::read_json(_path, m_ptree);
}

const std::string Config::getProp(const std::string& _prop, const std::string& _default) const
{
  return m_ptree.get<std::string>(_prop, _default);
}

const bool Config::getProp(const std::string& _prop, bool _default) const
{
  return m_ptree.get<bool>(_prop, _default);
}

const int Config::getProp(const std::string& _prop, const int _default) const
{
  return m_ptree.get<int>(_prop, _default);
}

