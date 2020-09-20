#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

typedef struct TCP_socket_struct* TCP_socket;

TCP_socket new_TCP_socket(char*,char*);
void free_TCP_socket(TCP_socket);

#endif
