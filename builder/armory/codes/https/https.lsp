(context 'HTTPS)

(define (login url data)
  (set 'login-cmd (format "curl -v --insecure -d \"%s\" %s 2>&1" data url))
  (exec login-cmd))

(define (post cookie url)
  (set 'post-cmd (append "curl -v --cookie \"" cookie "\" " url " 2>&1"))
  (println post-cmd)
  (exec post-cmd))

