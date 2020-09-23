#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "strings.h"

typedef struct TCP_socket_struct* TCP_socket;

TCP_socket new_TCP_server_socket(char*,char*);
char *get_address(TCP_socket);
int get_port(TCP_socket);
void TCP_socket_begin_listen(TCP_socket,int);
TCP_socket await_client_connection(TCP_socket);
string TCP_socket_recv(TCP_socket,int);
string TCP_socket_recvln(TCP_socket);
void TCP_send_string(TCP_socket,string);
void free_TCP_socket(TCP_socket);

#endif
