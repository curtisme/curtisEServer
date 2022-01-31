#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strings.h"
#include "re.h"
#include "utils.h"

#define MAX_MATCHES 10

struct regex_struct
{
    regex_t re;
};

regex
new_regex(char *pattern, int flags)
{
    regex new;
    new = smalloc(sizeof(*new), "new_regex");
    if (regcomp(&(new->re), pattern, REG_EXTENDED))
        fatal_error("regcomp fail in call to new_regex");
    return new;
}

match
new_match()
{
    match new;
    new = smalloc(sizeof(*new), "new_match");
    new->groups = smalloc(sizeof(*(new->groups)) * MAX_MATCHES, "new_match");
    new->is_match = 0;
    new->num_matches = 0;
}

match
regex_match(regex pattern, char *input)
{
    int i, len;
    match m = new_match();
    regmatch_t matches[MAX_MATCHES];
    int reti = regexec(&(pattern->re), input, MAX_MATCHES, matches, 0);
    if (!reti)
    {
        m->is_match = 1;
        for (i=0; i < MAX_MATCHES && matches[i].rm_so >= 0; i++)
        {
            len = matches[i].rm_eo - matches[i].rm_so;
            m->groups[i] = new_empty_string(len);
            memcpy(get_chars(m->groups[i]), input + matches[i].rm_so, len);
            set_length(m->groups[i], len);
            null_terminate(m->groups[i]);
            m->num_matches += 1;
        }
    }
    else if (reti == REG_NOMATCH)
        m->is_match = 0;
    else
        fatal_error("regexec error");
    return m;
}

void
free_match(match m)
{
    int i;
    for (i=0;i<m->num_matches;i++)
    {
        free_string(m->groups[i]);
    }
    free(m->groups);
    free(m);
}
void
free_regex(regex re)
{
    regfree(&(re->re));
    free(re);
}
