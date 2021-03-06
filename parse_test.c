#include "http_header_parser.h"

char const * const header = 
    "GET /user HTTP/1.1\r\n"
    "Host: localhost:4030\r\n"
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
    "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "Connection: keep-alive\r\n"
    "Upgrade-Insecure-Requests: 1\r\n";

#include "log.h"

int main() {
    struct HttpRequest request;
    consoleLogger.LPrintf(LOGGER_FULL_OK "%d", sizeof(request));
    
    enum HttpParseHeaderError error = HttpHeaderParse(header, &request); 
    (void) error;
    
    switch (request.method) { 
        case HTTP_METHOD_GET: {
            consoleLogger.LPrintf(LOGGER_FULL_OK "GET");
            break;
        }
        
        case HTTP_METHOD_POST: {
            consoleLogger.LPrintf(LOGGER_FULL_OK "POST");
            break;
        }
        
        default: {
            consoleLogger.LPrintf(LOGGER_FULL_OK "ANOTHER");
            break;
        }
    }
    
    #ifdef HTTP_ENABLE_HEADER_PARAM_ITERATOR
        struct HttpParamIterator iter = HttpCreateParamIterator(header);
        struct HttpKeyValue param = {0};
        enum HttpParamIteratorStatus res;
        
        while ((res = HttpHeaderIteratorGetNext(&iter, &param)) == HTTP_PARAM_ITERATOR_STATUS_OK) {
            consoleLogger.LPrintf("%.*s", param.key.len, param.key.ptr);
            consoleLogger.LPrintf("%.*s", param.value.len, param.value.ptr);
            consoleLogger.LPrintf("");
        }
        
        
    #endif
    
    
    //consoleLogger.LPrintf("%.*s", request.parametrs.user_agent.key.len, request.parametrs.user_agent.key.ptr);
    //consoleLogger.LPrintf("%.*s", request.parametrs.user_agent.value.len, request.parametrs.user_agent.value.ptr);
    
}