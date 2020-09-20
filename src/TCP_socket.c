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
    struct scokaddr_in address;
};

TCP_socket
new_TCP_socket(char *hostname, char *port)
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

