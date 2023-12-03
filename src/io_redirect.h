#ifndef IO_REDIRECT_H
#define IO_REDIRECT_H
#include <stddef.h>

int handle_io_redirection(size_t *argc, char *argv[], int *saved_stdout, int *saved_stdin);

#endif 