#include <stdio.h>
#include <string.h>

#include "echo.h"

int my_echo(int argc, char **argv) {
    // 检查是否有参数
    if (argc < 2) {
        printf("Usage: %s [message]\n", argv[0]);
        return 1;
    }
    int flag = 0;
    if(strcmp(argv[1], "-n") == 0)
        flag = 1;
    // 将命令行参数打印到终端
    for (int i = 1 + flag; i < argc; i++) {
        printf("%s", argv[i]);
        // 在除最后一个参数外的每个参数后面添加一个空格
        if (i < argc - 1) 
            printf(" ");
    }
    if(!flag)
        printf("\n");         // 输出换行符
    return 0;
}
