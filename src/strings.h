#ifndef STRINGS_H
#define STRINGS_H

typedef struct string_struct* string;

string new_empty_string(int capacity);
string new_string_from(char*);
void append_string(string, char*);
void append_char(string, char);
int string_length(string);
int string_compare(string, string);
char *get_chars(string);
int set_length(string,int);
string read_file_to_string(FILE*);
void free_string(string);

#endif
