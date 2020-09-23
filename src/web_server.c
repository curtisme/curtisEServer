#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"
#include "TCP_socket.h"
#include "http.h"
#include "web_server.h"

void handle_http_request(TCP_socket client);
void *request_handler_thread_fn(void*);

struct web_server_struct
{
    TCP_socket socket;
    string name;
    string webroot_path;
};

typedef struct {
    web_server server;
    TCP_socket client;
} thread_arg;

web_server
new_web_server(char *hostname, char *port)
{
    web_server new = smalloc(sizeof(*new), "new_web_server");
    new->socket = new_TCP_server_socket(hostname, port);
    return new;
}

void
web_server_serve(web_server server)
{
    TCP_socket client_socket;

    TCP_socket_begin_listen(server->socket, 5);
    while(1)
    {
        client_socket = await_client_connection(server->socket);
        handle_http_request(client_socket);
    }
}

void
handle_http_request(TCP_socket client)
{
    pthread_t tid;
    pthread_create(&tid,
            NULL,
            request_handler_thread_fn, 
            (void*)client);
}

void
*request_handler_thread_fn(void *arg)
{
    TCP_socket client = (TCP_socket)arg;
    http_request request;
    http_response response;

    /*response = new_string_from("HTTP/1.0 200 OK\r\n"
            "Server: curtisEServer\r\n\r\n"
            "<html><head><title>i am god</title></head>\r\n"
            "<body><h1><u>Curtis is a noble beast</u></h1>\r\n"
            "</br><h3>never forget it</h3></body>\r\n"
            "</html>\r\n");*/
    request = http_parse_request(client);
    response = http_process_request(request);
    printf("%s:%d %s /%s %s\n", get_address(client), get_port(client),
            get_method(request), get_uri(request),
            get_status(response));
    TCP_send_string(client, get_response_string(response));
    free_TCP_socket(client);
    free_http_request(request);
    free_http_response(response);
}


void
free_web_server(web_server server)
{
    free_TCP_socket(server->socket);
    free(server);
}

