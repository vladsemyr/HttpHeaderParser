#include "http_header_parser.h"



static const char * GetLineBegin(const char *ptr) {
    if (ptr == NULL)
        return NULL;
    
    while ((*ptr == '\n' || *ptr == '\r') && *ptr != '\0')
        ptr++;
    
    return ptr;
}

static const char * GetLineEnd(const char *ptr) {
    if (ptr == NULL)
        return NULL;
    
    while ((*ptr != '\n' && *ptr != '\r') && *ptr != '\0')
        ptr++;
    
    return ptr;
}

static const char* StrBeginsWith(const char* line, const char* pattern, size_t len) {
    // TODO: флаг регистронезависимость
    size_t i = 0;
    for (i = 0; i < len; ++i) {
        if (*pattern == '\0')
            return line;
        
        if (*line != *pattern)
            return NULL;
            
        line++;
        pattern++;
    }
    
    return NULL;
}

static const char * StrTrim(const char *line, size_t *len) {
    const char *front = line;
    const char *end = line + *len - 1;
    
    while (front != end) {
        if (*front == ' ')
            front++;
        else
            break;
    }
    
    while (end != front) {
        if (*end == ' ') // TODO: добавить флаг или что-то другое
            end--;
        else
            break;
    }
    
    end++;
    *len = end - front;
    return front;
}

static const char * StrFindFirst(const char *line, size_t len, char c) {
    for (int i = 0; i < len; ++i) {
        if (line[i] == c) {
            return line + i;
        }
    }
    
    return NULL;
}


#include <stdio.h>

static void ParseLine(const char *line_start, const char *line_end, struct HttpRequest *request) {
    if (line_start == NULL || line_end == NULL)
        return;
    
    size_t len = line_end - line_start;
    
    // TODO: проверка на `:`
    #define CREATE_PARAM(line_begin, request_param) {                       \
        const char * val = StrBeginsWith(line_start, line_begin ":", len);  \
        if (val != NULL) {                                                  \
            struct HttpKeyValue param;                                      \
            param.key.len = val - line_start - 1;                           \
            param.key.ptr = StrTrim(line_start, &param.key.len);            \
            param.value.len = line_end - val;                               \
            param.value.ptr = StrTrim(val, &param.value.len);               \
            request_param = param;                                          \
        }                                                                   \
    }
    
    #ifdef HTTP_HEADER_REQUEST_ACCEPT
        CREATE_PARAM(HTTP_HEADER_REQUEST_ACCEPT, request->parametrs.accept)
    #endif
    
    #ifdef HTTP_HEADER_REQUEST_ACCEPT_ENCODING
        CREATE_PARAM(HTTP_HEADER_REQUEST_ACCEPT_ENCODING, request->parametrs.accept_encoding)
    #endif
    
    #ifdef HTTP_HEADER_REQUEST_ACCEPT_LANGUAGE
        CREATE_PARAM(HTTP_HEADER_REQUEST_ACCEPT_LANGUAGE, request->parametrs.accept_language)
    #endif
    
    #ifdef HTTP_HEADER_REQUEST_HOST
        CREATE_PARAM(HTTP_HEADER_REQUEST_HOST, request->parametrs.host);
    #endif
    
    #ifdef HTTP_HEADER_REQUEST_CONNECTION
        CREATE_PARAM(HTTP_HEADER_REQUEST_CONNECTION, request->parametrs.connection);
    #endif
    
    #ifdef HTTP_HEADER_REQUEST_COOKIE
        CREATE_PARAM(HTTP_HEADER_REQUEST_COOKIE, request->parametrs.cookie);
    #endif
    
    #ifdef HTTP_HEADER_REQUEST_HOST
        CREATE_PARAM(HTTP_HEADER_REQUEST_HOST, request->parametrs.host);
    #endif
    
    #ifdef HTTP_HEADER_REQUEST_REFERER
        CREATE_PARAM(HTTP_HEADER_REQUEST_REFERER, request->parametrs.referer);
    #endif
    
    #ifdef HTTP_HEADER_REQUEST_USER_AGENT
        CREATE_PARAM(HTTP_HEADER_REQUEST_USER_AGENT, request->parametrs.user_agent);
    #endif
    
    #undef CREATE_PARAM
}

static enum HttpMethod HttpHeaderGetMethod(const char *line_start, const char *line_end, const char ** end_method_ptr) {
    const size_t first_line_len = line_end - line_start;
    const char * emp = NULL;
    enum HttpMethod method = HTTP_METHOD_UNSUPPORTED;
    
    // TODO: структура с соответствием enum - string
    if ((emp = StrBeginsWith(line_start, "GET", first_line_len)) != NULL) {
        method = HTTP_METHOD_GET;
    }
    else if ((emp = StrBeginsWith(line_start, "POST", first_line_len)) != NULL) {
        method = HTTP_METHOD_POST;
    }
    else if ((emp = StrBeginsWith(line_start, "HEAD", first_line_len)) != NULL) {
        method = HTTP_METHOD_HEAD;
    }
    
    *end_method_ptr = emp;
    return method;
}

struct HttpString HttpHeaderGetPath(const char * end_method_ptr, const char * line_end, const char ** path_end) {
    size_t path_protocol_len = line_end - end_method_ptr;
    const char *ps = StrTrim(end_method_ptr, &path_protocol_len);
    const char *pe = StrFindFirst(ps, path_protocol_len, ' ');
    const size_t path_len = pe - ps;
    
    *path_end = pe;
    struct HttpString ret = {
        .ptr = ps,
        .len = path_len,
    };
    return ret;
}

static enum HttpProtocol HttpHeaderGetProtocol(const char * path_ptr_end, const char * line_end) {
    size_t protocol_len = line_end - path_ptr_end;
    const char *protocol_start = StrTrim(path_ptr_end, &protocol_len);
    enum HttpProtocol protocol = HTTP_PROTOCOL_UNSUPORTED;
    
    if (StrBeginsWith(protocol_start, "HTTP/1.1", protocol_len + 1)) {
        protocol = HTTP_PROTOCOL_1_1;
    }
    
    return protocol;
}

enum HttpParseHeaderError HttpHeaderParse(const char *buffer, struct HttpRequest *request) {
    if (buffer == NULL)
        return HTTP_PARSE_HEADER_ERROR_NULL_BUFFER;
    
    // первая строка
    const char *line_start = GetLineBegin(buffer);
    const char *line_end = GetLineEnd(line_start);
    
    if (line_start == NULL || line_end == NULL)
        return HTTP_PARSE_HEADER_ERROR_FIRST_LINE;
    
    const char * method_ptr_end = NULL;
    enum HttpMethod method = HttpHeaderGetMethod(line_start, line_end, &method_ptr_end);
    if (method == HTTP_METHOD_UNSUPPORTED)
        return HTTP_PARSE_HEADER_ERROR_METHOD;
    
    const char * path_ptr_end = NULL;
    struct HttpString path = HttpHeaderGetPath(method_ptr_end, line_end, &path_ptr_end);
    if (path.ptr == NULL)
        return HTTP_PARSE_HEADER_ERROR_PATH;
    
    enum HttpProtocol protocol = HttpHeaderGetProtocol(path_ptr_end, line_end); // TODO: проверка на конец строки
    if (protocol == HTTP_PROTOCOL_UNSUPORTED)
        return HTTP_PARSE_HEADER_ERROR_PROTOCOL;
    
    request->method = method;
    request->protocol = protocol;
    request->path = path;
    
    
    
    do {
        ParseLine(line_start, line_end, request);
        
        //printf("|%.*s|\n", (int)param.param_name_len, param.param_name);
        //printf("    |%.*s|\n", (int)param.param_value_len, param.param_value);
        
        line_start = GetLineBegin(line_end);
        line_end = GetLineEnd(line_start); 
    } while (line_start != line_end);
    
    return HTTP_PARSE_HEADER_OK;
}