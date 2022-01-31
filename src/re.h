#ifndef RE_H
#define RE_H

typedef struct regex_struct* regex;
typedef struct
{
    int is_match;
    int num_matches;
    string *groups;
}* match;

regex new_regex(char *pattern, int flags);
match regex_match(regex pattern, char *input);
void free_match(match m);
void free_regex(regex re);

#endif
