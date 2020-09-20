#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

typedef struct TCP_socket_struct* TCP_socket;

TCP_socket new_TCP_server_socket(char*,char*);
void TCP_socket_begin_listen(TCP_socket,int);
TCP_socket await_client_connection(TCP_socket);
int TCP_socket_recv(TCP_socket,char*,int);
void TCP_send_string(TCP_socket,char*);
void free_TCP_socket(TCP_socket);

#endif
