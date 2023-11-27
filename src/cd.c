#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "cd.h"

int cd(int argc, char **argv){
    if(argc == 1){
        char *username = getlogin();
        char path[1024];
        snprintf(path, sizeof(path), "/home/%s", username);
        chdir(path);
    }
    else if(argc == 2){
        if(strcmp(argv[1], "~") == 0){
            char *username = getlogin();
            char path[1024];
            snprintf(path, sizeof(path), "/home/%s", username);
            chdir(path);
        }
        //else if(strcmp(argv[1], "-") == 0) 切换到上次所在的目录
        else{
            if(chdir(argv[1]) < 0){
                fprintf(stderr, "%s:No such file or directory",argv[0]);
            }
        }
        
    }
    else{
        fprintf(stderr, "%s:too many arguments", argv[0]);
    }
    return 0;
}

