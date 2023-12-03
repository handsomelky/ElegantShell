#ifndef PIPE_H
#define PIPE_H

#include "bst.h"
#include "main.h"
#include <glob.h>

int handle_pipe(char *linebuf, BST_NODE *bst_root,pid_t pid, int *saved_stdout, int *saved_stdin);

#endif
