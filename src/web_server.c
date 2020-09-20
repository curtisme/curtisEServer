#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "web_server.h"


struct web_server_struct
{
    TCP_socket socket;
   // struct sockaddr_in address;
};

web_server
new_web_server(char *hostname, char *port)
{
    web_server new = smalloc(sizeof(*new), "new_web_server");
    new->socket = new_TCP_socket(hostname, port);
    return new;
}

void
web_server_serve(web_server server)
{
    /*int new_socket;
    struct sockaddr_in client;
    socklen_t sin_size;
    pthread_t tid;*/

    TCP_socket_begin_listen(server->socket, 5);
    while(1)
    {
        /*sin_size = sizeof(struct sockaddr_in);
        new_socket = accept(server->socket_fd,
                (struct sockaddr*)&client, &sin_size);
        if (new_socket == -1)
            fatal_error("Failed to accept connection in call"
                    "to web_server_serve(web_server)");
        client_connection c;
        c.socket_fd = new_socket;
        c.client = client;
        pthread_create(&tid, NULL, handle_http_request, (void*)&c);*/
        handle_http_request(server->socket);
    }
}

/*void*
handle_http_request(void *client_data)
{
    char *s = "HTTP/1.0 200 OK\r\n"
        "Server: curtisEServer\r\n\r\n"
        "<html><head><title>i am god</title></head>\r\n"
        "<body><h1><u>Curtis is a noble beast</u></h1>\r\n"
        "</br><h3>never forget it</h3></body>\r\n"
        "</html>\r\n";
    int socket = ((client_connection*)client_data)->socket_fd;
    struct sockaddr_in client = ((client_connection*)client_data)->client;
    printf("new thread created\nclient address: %s:%d \n",
            inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    send(socket, s, strlen(s), 0);
    shutdown(socket, SHUT_RDWR);
    printf("thread end\n");
}*/

void
free_web_server(web_server server)
{
    free(server->socket);
    free(server);
}

