#include "io_redirect.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "io_redirect.h"

#define MAX_FILE_SIZE 1024  // 假设输入文件内容不超过1024字节

int handle_io_redirection(size_t *argc, char *argv[], int *saved_stdout, int *saved_stdin) {
    *saved_stdout = dup(STDOUT_FILENO);
    *saved_stdin = dup(STDIN_FILENO);
    int i;
    char inputStr[MAX_FILE_SIZE]; 

    for (i = 0; i < *argc; i++) {
        // 标准输出重定向
        if (strcmp(argv[i], ">") == 0) {  
            argv[i] = NULL;
            int fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            *argc-=2;

            return 0;
        } 
        // 标准输出追加重定向
        else if (strcmp(argv[i], ">>") == 0) {  
            argv[i] = NULL;
            int fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            *argc-=2;
            return 0;
        }
        // 标准输入重定向 
        else if (strcmp(argv[i], "<") == 0) {  
            int fd = open(argv[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            ssize_t bytesRead = read(fd, inputStr, MAX_FILE_SIZE - 1);
            if (bytesRead < 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            inputStr[bytesRead] = '\0';  // 确保字符串结束
            close(fd);

            // 移除重定向符号和文件名
            memmove(&argv[i], &argv[i + 2], (*argc - i - 2) * sizeof(char*));
            *argc -= 2;

            // 处理字符串并添加到 argv 中，使用换行符作为分隔符
            char *token = strtok(inputStr, "\n"); 
            while (token != NULL) {
                argv[*argc] = strdup(token); 
                (*argc)++;
                token = strtok(NULL, "\n");
            }

            // argv[*argc] = NULL;  // 确保 argv 结尾为 NULL

            return 0;  // 成功处理输入重定向
        }
        
    }
    return -1;
}
