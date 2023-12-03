#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "cd.h"

int cd(int argc, char **argv){
    char path[1024];
    if(argc == 1){      //cd
        char *username = getlogin();      //获取当前登录用户的用户名
        snprintf(path, sizeof(path), "/home/%s", username);
    }
    else if(argc == 2){    
        if(strcmp(argv[1], "~") == 0){     //cd ~
            char *username = getlogin();
            snprintf(path, sizeof(path), "/home/%s", username);
        }
        else
            snprintf(path, sizeof(path), "%s", argv[1]);   //cd + <dir>   
    }
    else
        fprintf(stderr, "%s:too many arguments", argv[0]);
    if(chdir(path) < 0)
        fprintf(stderr, "%s:No such directory",argv[0]);
    return 0;
}

 
