#!/usr/bin/newlisp

(load "curl.lsp")
(println (curl-get "http://movie.douban.com/"))
;; url example: http://movie.douban.com/subject/3326730/?from=hot_movie
(exit)
