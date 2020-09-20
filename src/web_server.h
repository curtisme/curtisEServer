#ifndef WEB_SERVER_H
#define WEB_SERVER_H

typedef struct web_server_struct* web_server;

web_server new_web_server(char*,char*);
void web_server_serve(web_server);
void free_web_server(web_server);

#endif
