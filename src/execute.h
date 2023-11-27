#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>
#include <sys/wait.h>
#include <time.h>

#include "cmd_st.h"

#include "main.h" 

void execute_command(cmd_st cmd, BST_NODE *bst_root,pid_t pid, int *saved_stdout, int *saved_stdin);

#endif 
