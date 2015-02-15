/*
 *author:yueruipeng@letv.com
 *date:2015/1/4
 *file:loadmanager.hpp
 *
 */

#ifndef COM_LETV_BIGDATA_MYSQL2HDFS_LOADMANAGER_HPP_
#define COM_LETV_BIGDATA_MYSQL2HDFS_LOADMANAGER_HPP_

class LoadManager
{

public:
  static LoadManager& Instance() { static LoadManager loadManager; return loadManager; }
  ~LoadManager();
  void startup();

private:
  LoadManager();
  LoadManager& operator=(const LoadManager&);

};

#endif
