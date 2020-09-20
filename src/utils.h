#ifndef UTILS_H
#define UTILS_H


void *smalloc(size_t, char *fail_message);
void *srealloc(void*, size_t, char *fail_message);
void fatal_error(char*);

#endif
