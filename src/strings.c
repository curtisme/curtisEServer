#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings.h"
#include "utils.h"

struct string_struct {
    int length,capacity;
    char *chars;
};

string
new_empty_string(int init_capacity) {
    string new;
    new = smalloc(sizeof(*new), "new_empty_string");
    new->length = 0;
    new->capacity = init_capacity;
    new->chars = smalloc((init_capacity + 1)*sizeof(char), "new_empty_string");
    new->chars[0] = '\0';
    return new;
}

string
new_string_from(char *src) {
    string new = new_empty_string(strlen(src));
    strcpy(new->chars, src);
    new->length = new->capacity;
    return new;
}

void
append_string(string s, char *src) {
    int src_len = strlen(src);
    if (s->length + src_len >= s->capacity) {
        s->capacity += src_len;
        s->chars = srealloc(s->chars, (s->capacity + 1)*sizeof(char), "append_string");
    }
    strcat(s->chars,src);
    s->length += src_len;
}

void
append_char(string s, char c) {
    if (s->length >= s->capacity) {
        s->capacity *= 2;
        s->chars = srealloc(s->chars, (s->capacity + 1)*sizeof(char), "append_char");
    }
    s->chars[s->length++] = c;
    s->chars[s->length] = '\0';
}

void
null_terminate(string s)
{
    if (s->length >= 0)
        s->chars[s->length] = '\0';
}

int
string_length(string s) {
    return s->length;
}

int
string_compare(string s1, string s2) {
    return strcmp(s1->chars, s2->chars);
}

int
string_is_empty(string s)
{
    return s->length < 1;
}

int
string_ends_with(string s, char c)
{
    return s->length > 0 &&
        s->chars[s->length-1] == c;
}

void
string_concat(string s, string src)
{
    int src_len = src->length;
    if (s->length + src_len >= s->capacity) {
        s->capacity += src_len;
        s->chars = srealloc(s->chars,
                (s->capacity + 1)*sizeof(char),
                "string_concat");
    }
    memcpy(s->chars + s->length, src->chars, src_len + 1);
    s->length += src_len;
}

char
*get_chars(string s) {
    return s->chars;
}

int
set_length(string s, int len)
{
    if (len >= s->capacity)
        len = s->capacity;
    s->length = len;
    s->chars[len] = '\0';
    return s->length;
}

string
read_to_string(FILE *fp)
{
    string out = new_empty_string(1);
    char c = fgetc(fp);
    while(!feof(fp))
    {
        append_char(out, c);
        c = fgetc(fp);
    }
    return out;
}

void free_string(string s) {
    free(s->chars);
    free(s);
}
