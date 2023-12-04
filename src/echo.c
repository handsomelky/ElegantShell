#include <stdio.h>
#include <string.h>

#include "echo.h"

int my_echo(int argc, char **argv) {
    // 检查是否有参数
    if (argc < 2) {
        printf("Usage: %s [message]\n", argv[0]);
        return 1;
    }
    // 检查是否是输出到文件的命令
    int flag_n = 0, flag2 = 0, local = 0;       // flag_n:是否输出换行符   flag2:输出到文件的情况
    for( int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-n") == 0)
            flag_n = 1;
        if(strcmp(argv[i], ">") == 0){
            flag2 = 1;
            local = i;
        }
        else if(strcmp(argv[i], ">>") == 0){ 
            flag2 = 2;
            local = i;
        }
    }
    if(flag2 == 0){
        // 将命令行参数打印到终端
        for (int i = 1 + flag_n; i < argc; i++) {
            for(int j = 0; j < strlen(argv[i]); j++){
                if(argv[i][j] != '"')
                    printf("%c", argv[i][j]);
            }
            // 在除最后一个参数外的每个参数后面添加一个空格
            if (i < argc - 1) 
                printf(" ");
        }
        if(!flag_n)
            printf("\n");         // 输出换行符
        return 0;
    }
    else{
        if(local + 1 == argc){
            char *filename = argv[local + 1];
            FILE *file = NULL;
            // 判断是覆盖还是追加
            if (flag2 == 1){
                file = fopen(filename, "w+");
            } 
            else if (flag2 == 2){     //+:不存在时创建文件
                file = fopen(filename, "a+");
            }
            if (file == NULL) {
                fprintf(stderr, "Error opening file");
                return 1;
            }
            // 将命令行参数打印到文件
            for (int i = 1 + flag_n; i < local; i++) {
                for(int j = 0; j < strlen(argv[i]); j++){
                    if(argv[i][j] !='"')
                        fprintf(file, "%c", argv[i][j]);
                }
                //在除最后一个参数外的每个参数后面添加一个空格
                if (i < local - 1) 
                    fprintf(file, " ");
            }
            if(!flag_n)
                printf("\n");         // 输出换行符
            fclose(file);
            return 1;   
        }
        else{
            fprintf(stderr,"Usage:%s [-e] 'content' [>] [>>] 'text.txt'",argv[0]);
            return 1;
        }
    }  
}
