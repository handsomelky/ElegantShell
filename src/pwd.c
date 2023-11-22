#include <stdio.h>
#include <stdlib.h>

#include "pwd.h"

#define PATH_MAX 1024

int pwd(int argc, char **argv){
    char buf[PATH_MAX] = {0};
    getcwd(buf, PATH_MAX);
    printf("%s\n", buf);

    return 0;
}