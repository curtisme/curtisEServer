#ifndef HTTP_H
#define HTTP_H

#include "strings.h"
#include "TCP_socket.h"

#define OK "200 OK"
#define NOT_FOUND "404 NOT FOUND"
#define BAD_REQUEST "400 BAD REQUEST"


typedef struct http_request_struct* http_request;
typedef struct http_response_struct* http_response;

http_request new_http_request();
http_response new_http_response();
http_request http_parse_request(TCP_socket);
http_response http_process_request(http_request);
char *get_method(http_request);
char *get_uri(http_request);
char *get_status(http_response);
string get_response_string(http_response);
void free_http_request(http_request);
void free_http_response(http_response);

#endif
