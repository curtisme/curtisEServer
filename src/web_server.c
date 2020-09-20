#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"
#include "TCP_socket.h"
#include "web_server.h"

void handle_http_request(TCP_socket client);
void *http_handler_thread_fn(void*);

struct web_server_struct
{
    TCP_socket socket;
};

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
    pthread_create(&tid, NULL, http_handler_thread_fn, (void*)client);
}

void
*http_handler_thread_fn(void *arg)
{
    TCP_socket client = (TCP_socket)arg;
    int recv_len;
    char buf[1024];

    char *s = "HTTP/1.0 200 OK\r\n"
        "Server: curtisEServer\r\n\r\n"
        "<html><head><title>i am god</title></head>\r\n"
        "<body><h1><u>Curtis is a noble beast</u></h1>\r\n"
        "</br><h3>never forget it</h3></body>\r\n"
        "</html>\r\n";
    recv_len = TCP_socket_recv(client, &buf, 1024);
    printf("%s\n\n%d  %c\n", buf, recv_len, buf[recv_len-1]);
    TCP_send_string(client, s);
    free_TCP_socket(client);
    sleep(5);
    printf("thread end\n");
}

void
free_web_server(web_server server)
{
    free_TCP_socket(server->socket);
    free(server);
}

