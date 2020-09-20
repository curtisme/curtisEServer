#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "utils.h"
#include "TCP_socket.h"

struct TCP_socket_struct
{
    int socket_fd;
    struct sockaddr_in address;
};

TCP_socket
new_TCP_server_socket(char *hostname, char *port)
{
    TCP_socket new = smalloc(sizeof(*new), "new_TCP_socket");
    int yes=1, new_sfd = socket(PF_INET, SOCK_STREAM, 0);
    if (new_sfd == -1)
        fatal_error("Failed to create new socket in call "
                "to new_TCP_socket(char*,char*)");
    if (setsockopt(new_sfd, SOL_SOCKET,
                SO_REUSEADDR, &yes, sizeof(int)) == -1)
        fatal_error("Failed to set socket option in call "
                "to new_TCP_socket(char*,char*).");
    new->address.sin_family = AF_INET;
    new->address.sin_port = htons(atoi(port));
    new->address.sin_addr.s_addr = 0;
    if (bind(new_sfd, (struct sockaddr*)&(new->address),
                sizeof(struct sockaddr)) == -1)
        fatal_error("Failed to bind address to socket in call "
                "to new_web_server(char*,char*).");
    new->socket_fd = new_sfd;
    return new;
}

void
TCP_socket_begin_listen(TCP_socket socket, int backlog)
{
    if (listen(socket->socket_fd, backlog) == -1)
        fatal_error("Failed to call listen(int,int) in call "
                "to TCP_socket_begin_listen(TCP_socket,int).");
}

TCP_socket
await_client_connection(TCP_socket server)
{
    int client_sfd;
    struct sockaddr_in client_addr;
    TCP_socket client_socket;
    socklen_t sin_size = sizeof(client_addr);

    client_sfd = accept(server->socket_fd,
            (struct sockaddr*)&client_addr, &sin_size);
    if (client_sfd == -1)
        fatal_error("Failed to accept TCP connection");
    client_socket = smalloc(sizeof(*client_socket),
            "await_client_connection");
    client_socket->socket_fd = client_sfd;
    client_socket->address = client_addr;
    return client_socket;
}

int
TCP_socket_recv(TCP_socket socket, char *buf, int buf_len)
{
    return recv(socket->socket_fd, buf, buf_len, 0);
}

void
TCP_send_string(TCP_socket out, char *s)
{
    send(out->socket_fd, s, strlen(s), 0);
}

void
free_TCP_socket(TCP_socket socket)
{
    shutdown(socket->socket_fd, SHUT_RDWR);
    free(socket);
}
