/*
 *author:yueruipeng@letv.com
 *date:2015/1/19
 *file:runcommand.hpp
 *
 */

#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_RUNCOMMAND_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_RUNCOMMAND_HPP_

bool runCommand(const std::string& _command);

#endif

