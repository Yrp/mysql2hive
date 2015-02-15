
;; @syntax (compile include-dirs src-dirs o-dir)
;; @parameter include-dirs a list contains one or more relative or absolute include directories
;; @parameter src-dirs a list contains one or more relative or absolute src dirs
(define (compile include-dirs src-dirs o-dir)
  (if (directory? o-dir)
      (file:clean-folder o-dir)
    (make-dir o-dir))
  (set 'path1 "")
  (dolist (path include-dirs)
    (set 'path1 (append path1 "-I" path " ")))

  (set 'cmd-template (format "c++ -g -std=c++11 %s `mysql_config --cxxflags` -Wall -DBOOST_LOG_DYN_LINK -o %s/" path1 o-dir))

  (dolist (dir src-dirs)
    (compile-dir dir cmd-template)
    )
  )

;; @syntax (compile-dir dir cmd-template)
;; @parameter dir one folder which contains many .cc or .cpp files
;; @parameter cmd-template the command template that has -g, -Wall and -I args
(define (compile-dir dir cmd-template)
  (set 'file-list (directory dir "\\.cc$|\\.cpp$"))
  (println "dir: " dir)
  (dolist (cc-path file-list)
    (set 'str (append cc-path ".o"))
    (set 'cmd (append cmd-template  str " -c "  dir "/" cc-path))
    (println cmd)
    (exec cmd))
  )

;; @syntax (dynamic-link o-dir bin-dir binary-name libs)
;; @parameter o-dir the direcotry includs all .o files
;; @parameter bin-dir the location of linked binary file
;; @libs the list of all dependencies
(define (dynamic-link o-dir bin-dir binary-name libs)
  (if (directory? bin-dir)
      (file:clean-folder bin-dir)
    (make-dir bin-dir))
  (set 'cmd "c++ -g")
  (set 'o-files (directory o-dir "\\.o"))
  (dolist (o-file o-files)
    (set 'cmd (append cmd " " (real-path o-dir) "/" o-file)))
  (set 'cmd (append cmd " -o " bin-dir "/" binary-name " `mysql_config --libs` -L$JAVA_HOME/jre/lib/amd64/server -L$HADOOP_HOME/lib/native/Linux-amd64-64 -rdynamic"))
  (dolist (lib libs)
     (if (= lib "pthread")
	 (set 'cmd (append cmd " -" lib))
       (set 'cmd (append cmd " -l" lib)))
     )
  (println cmd)
  (exec cmd)
  )

;; @syntax (static-link o-dir bin-dir binary-name libs)
;; @parameter o-dir the direcotry includs all .o files
;; @parameter bin-dir the location of linked binary file
;; @libs the list of all dependencies
(define (static-link o-dir bin-dir binary-name libs)
  (if (directory? bin-dir)
      (file:clean-folder bin-dir)
    (make-dir bin-dir))
  (set 'cmd "g++ -g ")
  (set 'o-files (directory o-dir "\\.o"))
  (dolist (o-file o-files)
    (set 'cmd (append cmd " " (real-path o-dir) "/" o-file)))
  (set 'cmd (append cmd " -o " bin-dir "/" binary-name " -static-libgcc -static-libstdc++ -static -L$JAVA_HOME/jre/lib/amd64/server -L/usr/lib/x86_64-linux-gnu"))
  (dolist (lib libs)
    (set 'cmd (append cmd " -l" lib))
    (println cmd))
  (exec cmd)
  )
