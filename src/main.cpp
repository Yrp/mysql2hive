/*
 *author:yueruipeng@letv.com
 *date:2014/12/25
 *file:main.cpp
 *
 */

#include "logger.hpp"
#include "mysql.hpp"
#include "tableloader.hpp"
#include "loadmanager.hpp"
#include "hdfs.h"
#include "config.hpp"

int main(int argc, char *argv[])
{

  if (argc != 3 || std::string(argv[1]) != "-c")
  {
    std::cout << "command format is error." << std::endl
            << "Please run program as the format:\n    " << argv[0] << " -c config_path" << std::endl;
    return 1;
  }
  LogInit();//��ʼ��log
//  Config::Instance().load(std::string(argv[2]));//��ʼ������
//  TableLoader::Instance().init();//������Ҫ�����ı���
//  LoadManager::Instance().startup();//����
LOG_ERR << "hello";


  return 0;
}


