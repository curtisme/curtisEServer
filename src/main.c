#include <stdio.h>
#include "web_server.h"

int
main(int argc, char **argv)
{
    web_server server;
    char *hostname, *port;

    if (argc < 2)
        printf("Please provide server port number.\n");

    else
    {
        hostname = "";
        port = argv[1];
        server = new_web_server(hostname, port);
        web_server_serve(server);
        printf("server closing...\n");
        free_web_server(server);
    }

    return 0;
}
