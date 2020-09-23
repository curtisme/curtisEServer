#include <stdio.h>
#include <stdlib.h>
#include "http.h"
#include "utils.h"

#define NOT_FOUND_PAGE "<!DOCTYPE html>\n<html>\n    <head>\n        " \
    "<title>404 n0t found!!1</title>\n" \
    "        <meta charset=\"utf-8\">\n        " \
    "<meta name=\"viewport\" content=\"width=device-width, " \
    "initial-scale=1.0\">\n    </head>\n" \
    "    <body>\n        <h1><u>404 Page not found</u></h1>\n" \
"    </body>\n</html>\n"

#define BAD_REQUEST_PAGE "<!DOCTYPE html>\n<html>\n    <head>\n        " \
    "<title>400 b4d request!!1</title>\n" \
    "        <meta charset=\"utf-8\">\n        " \
    "<meta name=\"viewport\" content=\"width=device-width, " \
    "initial-scale=1.0\">\n    </head>\n" \
    "    <body>\n        <h1><u>400 Invalid HTTP request</u></h1>\n" \
"    </body>\n</html>\n"

enum http_method
{
    GET,
    HEAD,
    POST,
    INVALID
};

enum parse_state
{
    request_line_start,

    method_G,
    method_GE,
    method_GET,
    method_H,
    method_HE,
    method_HEA,
    method_HEAD,
    method_P,
    method_PO,
    method_POS,
    method_POST,

    expecting_uri,
    reading_uri,

    done,

    request_line_invalid
};

struct http_request_struct
{
    enum http_method method;
    string method_string;
    string uri;
};

struct http_response_struct
{
    string status;
    string body;
    string full_response;
};


void generate_full_response(http_response, int);

http_request
new_http_request()
{
    http_request new;
    new = smalloc(sizeof(*new), "new_http_request");
    new->method = INVALID;
    new->uri = new_empty_string(1);
    return new;
}

http_response
new_http_response()
{
    http_response new = smalloc(sizeof(*new), "new_http_response");
    return new;
}
http_request
http_parse_request(TCP_socket client)
{
    http_request new = new_http_request();
    string firstline = TCP_socket_recvln(client);
    char *c = get_chars(firstline);
    enum parse_state state = request_line_start;
    while(*c && state != done)
    {
        switch(state)
        {
            case request_line_start:
                switch(*c)
                {
                    case 'G':
                        state = method_G;
                        break;
                    case 'H':
                        state = method_H;
                        break;
                    case 'P':
                        state = method_P;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case method_G:
                switch(*c)
                {
                    case 'E':
                        state = method_GE;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case method_H:
                switch(*c)
                {
                    case 'E':
                        state = method_HE;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case method_P:
                switch(*c)
                {
                    case 'O':
                        state = method_PO;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case method_GE:
                switch(*c)
                {
                    case 'T':
                        state = method_GET;
                        new->method = GET;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case method_HE:
                switch(*c)
                {
                    case 'A':
                        state = method_HEA;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case method_PO:
                switch(*c)
                {
                    case 'S':
                        state = method_POS;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case method_HEA:
                switch(*c)
                {
                    case 'D':
                        state = method_HEAD;
                        new->method = HEAD;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case method_POS:
                switch(*c)
                {
                    case 'T':
                        state = method_POST;
                        new->method = POST;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case method_GET:
            case method_HEAD:
            case method_POST:
                switch(*c)
                {
                    case ' ':
                        state = expecting_uri;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;

            case expecting_uri:
                switch(*c)
                {
                    case '/':
                        state = reading_uri;
                        break;
                    default:
                        state = request_line_invalid;
                        break;
                }
                break;
            case reading_uri:
                switch(*c)
                {
                    case ' ':
                        state = done;
                        break;
                    default:
                        append_char(new->uri, *c);
                        break;
                }
                break;
            default:
                state = request_line_invalid;
                break;
        }
        if (state == request_line_invalid)
        {
            new->method = INVALID;
            break;
        }
        c++;
    }
    switch(new->method)
    {
        case GET:
            new->method_string = new_string_from("GET");
            break;
        case HEAD:
            new->method_string = new_string_from("HEAD");
            break;
        case POST:
            new->method_string = new_string_from("POST");
            break;
        default:
            new->method_string = new_string_from("INVALID");
            break;
    }
    return new;
}

http_response
http_process_request(http_request request)
{
    http_response response = new_http_response();
    response->status = new_string_from(BAD_REQUEST);
    response->body = new_string_from(BAD_REQUEST_PAGE);
    generate_full_response(response, 1);
   /* FILE *fp;
    string response_text = new_string_from("HTTP/1.0 ");
    string resource_path = new_string_from("./");
    append_string(resource_path, get_chars(request->uri));
    if (string_ends_with(request->uri, '/') || 
            string_is_empty(request->uri))
        append_string(resource_path, "index.html");
    fp = fopen(get_chars(resource_path));
    switch(request->method)
    {
        case INVALID:
            append_string(response_text, BAD_REQUEST);
            break;
        case HEAD:
        case GET:
            if (fp == NULL)
            {
                append_string(response_text, NOT_FOUND);
                append_string(response_text, "\r\n");
                append_string(response_text, server_name);
            }
            else
            {
                append_string(response_text, OK);
                append_string(response_text, "\r\n");
                append_string(response_text, server_name);
            }
    }*/
    return response;
}

void
generate_full_response(http_response response, int attach_body)
{
    response->full_response = new_string_from("HTTP/1.0 ");
    append_string(response->full_response,
		    get_chars(response->status));
    append_string(response->full_response, "\r\n");
    append_string(response->full_response,
            "Server: curtisEServer\r\n\r\n");
    if (attach_body)
        append_string(response->full_response,
                get_chars(response->body));
}

char
*get_method(http_request request)
{
    return get_chars(request->method_string);
}

char
*get_uri(http_request request)
{
    return get_chars(request->uri);
}

char
*get_status(http_response response)
{
    return get_chars(response->status);
}

string
get_response_string(http_response response)
{
    return response->full_response;
}

void
free_http_request(http_request hr)
{
    free_string(hr->method_string);
    free_string(hr->uri);
    free(hr);
}

void
free_http_response(http_response response)
{
    free_string(response->status);
    free_string(response->body);
    free(response);
}
