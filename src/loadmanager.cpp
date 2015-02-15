/*
 *author:yueruipeng@letv.com
 *date:2015/1/4
 *file:loadmanager.cpp
 *
 */

#include "loadmanager.hpp"
#include "loader.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "tableloader.hpp"
#include <vector>

LoadManager::LoadManager()
{
}

LoadManager::~LoadManager()
{
}

void LoadManager::startup()
{  
  const int threadNum = Config::Instance().getProp("thread_num", 1);
  const int fifoSize = Config::Instance().getProp("fifo_size", 100);
  const int intervalTime = Config::Instance().getProp("interval_time", 0);
  int loadtimes = 0;
  
  while (true)
  {
    LOG_INFO << "the " << loadtimes << "th loadManager startup.";

    TableLoader::Instance().init();//加载需要导出的表名

    size_t begin = time(0);
    std::vector<Loader*> loaders;
    for(int i=0; i<threadNum; i++)
    {
      Loader* loader = new Loader(i, fifoSize);
      loaders.push_back(loader);
      loader->run();   
    }
  
    sleep(5);
    
    for(unsigned int i=0; i< loaders.size(); i++)
    {
      while(!loaders[i]->waitForShutdown(2000)) {}
      delete loaders[i];
    }

    size_t end = time(0);
    LOG_INFO << "the " << loadtimes++ << "th loadManager end. [used time: " << end-begin << "s.]";
    
    if (intervalTime == 0)
    {
      break;
    }
    
    for (int i= intervalTime; i > 0; i--)
    {
      LOG_INFO << "there are " << i << " minutes before the next loading.";
      sleep(60);
    }
   
  }
    
}