#!/usr/bin/newlisp 

;; init
(load "armory_config.lsp")
(load "build_config.lsp")

;; compile all c++ files to .o file in ./o folder
(load "g++.lsp")
(compile include-paths src-paths o-dir)

;; link all .o files
(dynamic-link o-dir bin-dir binary-name libs)


(exit)
