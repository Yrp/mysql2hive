;; @module mongo.lsp 
;; @description MongoDB v2.6.1 interface (tested on MySQL 2.6.1)

;; @author Dean Chen 2014-05-15

(context 'Mongo)

(set 'files '(
	      "/usr/local/lib/libmongoc-1.0.so"))

(set 'library (files (or 
		      (find true (map file? files)) 
		      (throw-error "cannot find libmongoc library"))))

(import "/usr/local/lib/libbson-1.0.so")
(import "/usr/local/lib/libbson-1.0.so" "bson_new")
(import "/usr/local/lib/libbson-1.0.so" "bson_append_utf8")

(import library "mongoc_client_new" "void*")
(import library "mongoc_client_get_collection")
(import library "mongoc_collection_find")
;;(import library "mongoc_collection_find" "void*" "unsigned int" "unsigned int" "unsigned int" "unsigned int" "void*" "void*" "void*")

(define (connect url)
  (mongoc_client_new url))

;; return mongoc_collection_t 
(define (get_coll session db_name coll_name)
  (mongoc_client_get_collection session db_name, coll_name))

;; mongoc_cursor_t *
;; mongoc_collection_find (mongoc_collection_t       *collection,
;;                        mongoc_query_flags_t       flags,
;;                        uint32_t                   skip,
;;                        uint32_t                   limit,
;;                        uint32_t                   batch_size,
;;                        const bson_t              *query,
;;                        const bson_t              *fields,
;; const mongoc_read_prefs_t *read_prefs);

;; typedef enum
;; {
;;    MONGOC_QUERY_NONE              = 0,
;;    MONGOC_QUERY_TAILABLE_CURSOR   = 1 << 1,
;;    MONGOC_QUERY_SLAVE_OK          = 1 << 2,
;;    MONGOC_QUERY_OPLOG_REPLAY      = 1 << 3,
;;    MONGOC_QUERY_NO_CURSOR_TIMEOUT = 1 << 4,
;;    MONGOC_QUERY_AWAIT_DATA        = 1 << 5,
;;    MONGOC_QUERY_EXHAUST           = 1 << 6,
;;    MONGOC_QUERY_PARTIAL           = 1 << 7,
;; } mongoc_query_flags_t;