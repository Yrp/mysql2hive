/*
 *author:yueruipeng@letv.com
 *date:2015/1/5
 *file:stringutil.hpp
 *
 */

#include "stringutil.hpp"
#include <string>
#include <vector>

int split_first_of(const std::string& in, std::vector<std::string>& out, const std::string& by)
{
  out.clear();

  if (in.empty())
  {
    return 0;
  }

  size_t pos = 0;
  while ( true )
  {
    size_t nextPos = in.find_first_of(by, pos);
    if (nextPos == std::string::npos)
    {
      out.push_back(std::string(in.substr(pos)));
      break;
    }
    out.push_back(std::string(in.substr(pos, nextPos - pos)));

    if (nextPos == in.size()-1)
    {
      break;
    }
    pos = nextPos + 1;
  }

  return out.size();

}

int split(const std::string& in, std::vector<std::string>& out, const std::string& by)
{
  out.clear();

  if (in.empty())
  {
    return 0;
  }

  size_t pos = 0;
  while ( true )
  {
    size_t nextPos = in.find(by, pos);
    if (nextPos == std::string::npos)
    {
      out.push_back(std::string(in.substr(pos)));
      break;
    }
    out.push_back(std::string(in.substr(pos, nextPos - pos)));

    if (nextPos == in.size()-1)
    {
      break;
    }
    pos = nextPos + 1;
  }

  return out.size();

}

int split(const std::string& in, std::set<std::string>& out, const std::string& by)
{	
  out.clear();

	if (in.empty())
	{
		return 0;
	}

	size_t pos = 0;
	while ( true )
	{
		size_t nextPos = in.find(by, pos);
		if (nextPos == std::string::npos)
		{
			out.insert(std::string(in.substr(pos)));
			break;
		}
		out.insert(std::string(in.substr(pos, nextPos - pos)));

		if (nextPos == in.size()-by.size())
		{
			break;
		}
		pos = nextPos + by.size();
	}

	return out.size();

}
