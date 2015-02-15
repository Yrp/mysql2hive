#!/usr/bin/newlisp
;;(trace true)
(load "mongo.lsp")
(set 'client (Mongo:connect "mongodb://127.0.0.1/"))
(set 'coll (Mongo:get_coll client "kaimei" "user"))

; check endianess of the host CPU 
;;(set 'MONGOC_QUERY_NONE (pack "b" 0)) ;; in c, it is an enum value, must be signed integer 32 bits
(set 'query (Mongo:bson_new))

;;(set 'key "email")
;;(set 'key_length (length key))
;;(set 'value "shu_chen@esri.com")
;;(set 'value_length (length value))

;;(Mongo:bson_append_utf8 query key key_length value value_length)
(Mongo:mongoc_collection_find(coll
			      (int (get-string (get-long (int 0))))
			      0
			      0
			      0
			      query
			      nil
			      nil))

(exit)
