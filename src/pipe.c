#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cmd_st.h"
#include "execute.h"

#define DELIMS " \t\n"
static void parse(char *line, struct cmd_st *res) {
    char *tok;
    int i = 0;
    while(1) {
        tok = strsep(&line, DELIMS);
        if(tok == NULL)
            break;
        if(tok[0] == '\0')
            continue;

        // 检测后台运行程序
        if(i != 0 && strcmp(tok, "&") == 0) {
            res->background = 1; 
            continue;
        }

        glob(tok, GLOB_NOCHECK|GLOB_APPEND*i, NULL, &res->globres);
        i = 1;
    }
}

int handle_pipe(char *linebuf, BST_NODE *bst_root,pid_t pid, int *saved_stdout, int *saved_stdin) {

    int pipefd[2];
    pid_t pid1, pid2,pid3,pid4;
    int i;
    struct cmd_st left_cmd,right_cmd;
    char *left_cmd_str, *right_cmd_str;
    char *pipe_symbol = strchr(linebuf, '|');


    // 如果没有找到管道，返回0
    if (!pipe_symbol) {
        puts(linebuf);
        return 0;
    }

    // 分割字符串为左右命令
    *pipe_symbol = '\0';  // 在管道符号位置放置字符串结束符
    left_cmd_str = linebuf;
    right_cmd_str = pipe_symbol + 1;

    // 创建cmd结构体并解析左右命令

    parse(left_cmd_str, &left_cmd);
    parse(right_cmd_str, &right_cmd);

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

        fflush(stdout);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // 执行管道前半部分的命令
        
        execvp(left_cmd.globres.gl_pathv[0], left_cmd.globres.gl_pathv);

        perror("execvp");
        exit(EXIT_FAILURE);
        // execute_command(left_cmd,bst_root,pid3,saved_stdout,saved_stdin);
        

        
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
            execvp(right_cmd.globres.gl_pathv[0], &right_cmd.globres.gl_pathv);

            perror("execvp");
            exit(EXIT_FAILURE);
            // execute_command(right_cmd,bst_root,pid4,saved_stdout,saved_stdin);
            

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
