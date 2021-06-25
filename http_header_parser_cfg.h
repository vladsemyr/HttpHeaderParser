#ifndef HTTP_HEADER_PARSER_CFG
#define HTTP_HEADER_PARSER_CFG

//#define HTTP_HEADER_REQUEST_ACCEPT              "Accept"
//#define HTTP_HEADER_REQUEST_ACCEPT_ENCODING     "Accept-Encoding"
//#define HTTP_HEADER_REQUEST_ACCEPT_LANGUAGE     "Accept-Language"
//#define HTTP_HEADER_REQUEST_CONNECTION          "Connection"
#define HTTP_HEADER_REQUEST_COOKIE              "Cookie"
//#define HTTP_HEADER_REQUEST_HOST                "Host"
//#define HTTP_HEADER_REQUEST_REFERER             "Referer"
//#define HTTP_HEADER_REQUEST_USER_AGENT          "User-Agent"


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

#endif