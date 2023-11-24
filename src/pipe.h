#ifndef PIPE_H
#define PIPE_H

#include <glob.h>

int handle_pipe(int arg_count, char *arg_vector[], int *saved_stdout, int *saved_stdin);

#endif
