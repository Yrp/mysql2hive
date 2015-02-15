(set 'include-paths
     (list "../include"
      "../thirdparty/hdfs/include"))

(set 'src-paths
     (list "../src"
	   ))
(set 'libs
     (list "pthread"
	   "boost_log"
	   "boost_log_setup"
	   "boost_system"
	   "boost_thread"
	   "boost_filesystem"
	   "hdfs"
	   "jvm"
	   ))

(set 'o-dir "./object")
(set 'bin-dir "../bin")
(set 'binary-name "mysql2hive")

