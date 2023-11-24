#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int handle_pipe(int arg_count, char *arg_vector[], int *saved_stdout, int *saved_stdin) {
    int pipefd[2];
    pid_t pid1, pid2;
    int i;

    // 查找命令中的管道符号 '|'
    for (i = 0; i < arg_count; i++) {
        if (strcmp(arg_vector[i], "|") == 0) {
            break;
        }
    }

    // 如果没有找到管道，返回0
    if (i == arg_count) {
        return 0;
    }

    // 创建管道
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建第一个子进程
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // 第一个子进程
        close(pipefd[0]); 

        // 将 STDOUT 重定向到管道的写端
        *saved_stdout = dup(STDOUT_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);

        fflush(stdout);
        close(pipefd[1]);

        // 执行管道前半部分的命令
        arg_vector[i] = NULL;
        execvp(arg_vector[0], arg_vector);

        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // 父进程
        close(pipefd[1]); 

        // 创建第二个子进程
        pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {
            // 第二个子进程
            close(pipefd[1]); // 再次确保写端被关闭

            // 将 STDIN 重定向到管道的读端
            *saved_stdin = dup(STDIN_FILENO);
            dup2(pipefd[0], STDIN_FILENO);
            fflush(stdout);
            close(pipefd[0]);

            // 执行管道后半部分的命令
            execvp(arg_vector[i + 1], &arg_vector[i + 1]);

            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            // 父进程继续
            close(pipefd[0]); // 关闭读端
            
        }
        // 等待两个子进程完成
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        return 1;
    }
}
