#ifndef CMD_ST_H
#define CMD_ST_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>
#include <sys/wait.h>
#include <time.h>

typedef struct cmd_st {
    glob_t globres;
    int background;
} cmd_st;

#endif