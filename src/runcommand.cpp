/*
 *author:yueruipeng@letv.com
 *date:2015/1/19
 *file:runcommand.cpp
 *
 */
 
#include "runcommand.hpp"

bool runCommand(const std::string& _command)
{
  pid_t status = system(_command.c_str());  
  
  if (-1 != status && WIFEXITED(status) && 0 == WEXITSTATUS(status))  
  {  
    return true;
  }  
  
  return false;  
}
