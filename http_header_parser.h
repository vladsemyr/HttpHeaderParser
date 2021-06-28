#ifndef HTTP_HEADER_PARSER
#define HTTP_HEADER_PARSER

#include <stddef.h>
#include "http_header_parser_cfg.h"

// GET / HTTP/1.1
// Host: localhost:4030
// User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
// Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3
// Accept-Encoding: gzip, deflate
// Connection: keep-alive
// Upgrade-Insecure-Requests: 1
// G�m


enum HttpMethod {
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_UNSUPPORTED,
};

enum HttpProtocol{
    HTTP_PROTOCOL_1_1,
    HTTP_PROTOCOL_UNSUPORTED,
};

#ifdef HTTP_ENABLE_HEADER_PARAM_ITERATOR
enum HttpParamIteratorStatus {
    HTTP_PARAM_ITERATOR_STATUS_OK = 0,
    HTTP_PARAM_ITERATOR_STATUS_END,
    HTTP_PARAM_ITERATOR_STATUS_ERROR,
};

struct HttpParamIterator{
    const char * line_start;
    const char * line_end;
    enum HttpParamIteratorStatus status;
};

#endif

struct HttpString {
    char const *ptr;
    size_t len;
};

struct HttpKeyValue {
    struct HttpString key;
    struct HttpString value;
};


struct HttpRequest {
    enum HttpMethod method;
    struct HttpString path;
    enum HttpProtocol protocol;
    
    #ifdef HTTP_HEADER_REQUEST_DEFINED_ANY
    struct {
        #ifdef HTTP_HEADER_REQUEST_ACCEPT
            struct HttpKeyValue accept;
        #endif
        
        #ifdef HTTP_HEADER_REQUEST_ACCEPT_ENCODING
            struct HttpKeyValue accept_encoding;
        #endif
        
        #ifdef HTTP_HEADER_REQUEST_ACCEPT_LANGUAGE
            struct HttpKeyValue accept_language;
        #endif
        
        #ifdef HTTP_HEADER_REQUEST_CONNECTION
            struct HttpKeyValue connection;
        #endif
        
        #ifdef HTTP_HEADER_REQUEST_COOKIE
            struct HttpKeyValue cookie;
        #endif
        
        #ifdef HTTP_HEADER_REQUEST_HOST
            struct HttpKeyValue host;
        #endif
        
        #ifdef HTTP_HEADER_REQUEST_REFERER
            struct HttpKeyValue referer;
        #endif
        
        #ifdef HTTP_HEADER_REQUEST_USER_AGENT
            struct HttpKeyValue user_agent;
        #endif
        
        
    } parametrs;
    #endif
};



enum HttpParseHeaderError {
    HTTP_PARSE_HEADER_OK = 0,
    HTTP_PARSE_HEADER_ERROR_NULL_BUFFER,
    HTTP_PARSE_HEADER_ERROR_FIRST_LINE,
    HTTP_PARSE_HEADER_ERROR_METHOD,
    HTTP_PARSE_HEADER_ERROR_PATH,
    HTTP_PARSE_HEADER_ERROR_PROTOCOL,
};


enum HttpParseHeaderError HttpHeaderParse(const char *buffer, struct HttpRequest *request) ;

#ifdef HTTP_ENABLE_HEADER_PARAM_ITERATOR
    struct HttpParamIterator HttpCreateParamIterator(const char *buffer);
    enum HttpParamIteratorStatus HttpHeaderIteratorGetNext(struct HttpParamIterator *iterator, struct HttpKeyValue *param);
#endif


#endif