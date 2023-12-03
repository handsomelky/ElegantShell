#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int my_mkdir(int argc, char **argv){
    if (argc == 2) {
        // 创建目录
        if (mkdir(argv[1], 0777) == 0) {
            printf("Directory created successfully.\n");
            return 0;  // 表示成功
        } 
        else {
            perror("Error creating directory");
            return 1;  // 表示错误
        }
    }
    else if(argc == 3){
        if(strcmp(argv[1], "-p") == 0){
            char *tmp = strdup(argv[2]);
            char *p = tmp;
            int status = 0;
            // 逐级检查和创建目录
            while (*p != '\0') {
                if (*p == '/') {
                    *p = '\0'; // 将斜杠替换为字符串终止符
                    // 检查目录是否存在
                    struct stat st;
                    if (stat(tmp, &st) != 0) {
                        // 如果目录不存在，则创建目录
                        if (mkdir(tmp, 0777) != 0) {
                            status = -1;
                            break;
                        }
                    } 
                    else if (!S_ISDIR(st.st_mode)) {
                        // 如果路径存在但不是目录，则返回错误
                        status = -1;
                        break;
                    }
                    *p = '/'; // 恢复原来的斜杠
                }
                p++;
            }
            // 创建最终目录
            if (status == 0 && mkdir(tmp, 0777) != 0) {
                status = -1;
            }
            free(tmp);
        }
        else{
            fprintf(stderr, "查询用法:help --%s\n", argv[0]);
            return 1;  // 表示错误
        }
    }
    else{
        fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
        return 1;  // 表示错误
    }
    return 0;
}