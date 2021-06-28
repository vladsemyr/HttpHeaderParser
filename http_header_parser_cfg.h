#ifndef HTTP_HEADER_PARSER_CFG
#define HTTP_HEADER_PARSER_CFG

//#define HTTP_HEADER_REQUEST_ACCEPT              "Accept"
//#define HTTP_HEADER_REQUEST_ACCEPT_ENCODING     "Accept-Encoding"
//#define HTTP_HEADER_REQUEST_ACCEPT_LANGUAGE     "Accept-Language"
//#define HTTP_HEADER_REQUEST_CONNECTION          "Connection"
//#define HTTP_HEADER_REQUEST_COOKIE              "Cookie"
//#define HTTP_HEADER_REQUEST_HOST                "Host"
//#define HTTP_HEADER_REQUEST_REFERER             "Referer"
//#define HTTP_HEADER_REQUEST_USER_AGENT          "User-Agent"

#define HTTP_ENABLE_HEADER_PARAM_ITERATOR

#if \
    defined(HTTP_HEADER_REQUEST_ACCEPT)             || \
    defined(HTTP_HEADER_REQUEST_ACCEPT_ENCODING)    || \
    defined(HTTP_HEADER_REQUEST_ACCEPT_LANGUAGE)    || \
    defined(HTTP_HEADER_REQUEST_CONNECTION)         || \
    defined(HTTP_HEADER_REQUEST_COOKIE)             || \
    defined(HTTP_HEADER_REQUEST_HOST)               || \
    defined(HTTP_HEADER_REQUEST_REFERER)            || \
    defined(HTTP_HEADER_REQUEST_USER_AGENT)

    #define HTTP_HEADER_REQUEST_DEFINED_ANY
#endif

#if defined(HTTP_HEADER_REQUEST_DEFINED_ANY) && defined(HTTP_ENABLE_HEADER_PARAM_ITERATOR)
    #warning Do you really want to use param iterator and parsing parametrs to struct
#endif

#endif