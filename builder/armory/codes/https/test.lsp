#!/usr/bin/newlisp


(define (login url data)
  (set 'login-cmd (format "curl -v --insecure -d \"%s\" %s 2>&1" data url))
  (exec login-cmd))

(define (post cookie url)
  (set 'post-cmd (append "curl -v --cookie \"" cookie "\" " url " 2>&1"))
  (println post-cmd)
  (exec post-cmd))


(set 'r (login "https://www.douban.com/accounts/login" "source=movie&redir=http://movie.douban.com/&form_email=7266529@qq.com&form_password=Crystal_45_liang&user_login=登录"))
(println r)


;;dbcl2= might be session id



(exit)
