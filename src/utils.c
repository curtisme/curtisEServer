#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void
*smalloc(size_t size, char *fail_message) {
    void *new = malloc(size);
    if (new == NULL) {
        fprintf(stderr, "Memory allocation failed! %s\n", fail_message);
        exit(EXIT_FAILURE);
    }
    return new;
}

void
*srealloc(void *old, size_t size, char* fail_message) {
    void *new = realloc(old, size);
    if (new == NULL) {
        fprintf(stderr, "Memory allocation failed! %s\n", fail_message);
        exit(EXIT_FAILURE);
    }
    return new;
}

void
safe_free(void **d) {
    if (*d != NULL) {
        free(*d);
        *d = NULL;
    }
}

void
fatal_error(char *error_msg)
{
    fprintf(stderr, "%s\n", error_msg);
    exit(EXIT_FAILURE);
}
