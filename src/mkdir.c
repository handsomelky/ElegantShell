#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mkdir.h"

int create(char *dir);

int my_mkdir(int argc, char **argv){
    int status = 0;   //记录出错原因：0代表没错，1代表目录已经存在,-1代表其他错误
    if(argc < 2){
        fprintf(stderr, "Usage: %s [dirction/name]", argv[0]);
            return 1;  // 表示错误
    }
    else{
        if(strcmp(argv[1], "-p") == 0){       //mkdir -p
            for(int i = 2; i < argc; i++){
                status = 0;
                char *tmp = strdup(argv[i]);
                char *p = tmp;
                while (*p != '\0' && status != -1) {   // 逐级检查和创建目录 
                    if (*p == '/'){   //提取每一级目录                     
                        *p = '\0'; 
                        status = create(tmp);
                        *p = '/'; // 恢复原来的斜杠
                    }
                    p++;
                }
                // 创建最终目录
                if (status == 0)
                    create(tmp);
                free(tmp); 
            }       
        }
        else{
            for(int i = 1; i < argc; i++){
                status = 0;
                status = create(argv[i]);
            }
        }
        if(status !=0){
           if(status == -1)
               fprintf(stderr, "查询用法:help --mkdir");  
           else if(status == 1)
               fprintf(stderr,"direction has already existed");
           return 1;
        }
    }
    return 0;
}

int create(char *dir)      //创建目录
{
    int status = 0;    //记录出错原因：0代表没错，1代表目录已经存在,-1代表其他错误
    struct stat st;
    if (stat(dir, &st) !=0) {    //目录不存在
        if (mkdir(dir, 0777) != 0)   //创建失败
            status = -1;
    } 
    else       //目录存在
        status = 1;
    return status;
}
