#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "main.h"

#define DELIMS " \t\n" 
BST_NODE *bst_root = NULL;


static void prompt(void) {
    //char *currentDir = getenv("PWD");   获取的是环境路径，而不是当前
    char *homeDir = getenv("HOME");
    char hostname[128];
    char username[128];
    char currentDir[1024];
    if(getcwd(currentDir, sizeof(currentDir)) == NULL){
        perror("getcwd");
        exit(EXIT_FAILURE);
    }


    if(currentDir == NULL || homeDir == NULL) {
        perror("getenv()");
        exit(1);
    }
    // 使用 ~ 替换 home 路径
    if(strncmp(currentDir, homeDir, strlen(homeDir)) == 0) {
        char tempPath[128];
        sprintf(tempPath, "~%s", currentDir + strlen(homeDir));
        strcpy(currentDir, tempPath);
    }

    gethostname(hostname, 128);
    getlogin_r(username, 128);

    printf("%s@%s:%s$ ", username, hostname, currentDir);
}


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


int main(void) {
    char *linebuf = NULL;
    size_t linebuf_size = 0;
    struct cmd_st cmd;
    pid_t pid;

    bst_insert(&bst_root, "hello", hello);
    bst_insert(&bst_root, "tree", mytree);
    bst_insert(&bst_root, "wc", wc);
    bst_insert(&bst_root, "ls", ls);
    bst_insert(&bst_root, "cd", cd);
    bst_insert(&bst_root, "pwd", pwd);
    bst_insert(&bst_root, "help", display_help);
    bst_insert(&bst_root, "ps", print_process_info);
    bst_insert(&bst_root, "echo", my_echo);
    bst_insert(&bst_root, "cat", cat);
    bst_insert(&bst_root, "_cp", my_cp);
    bst_insert(&bst_root, "_mv", my_mv);
    bst_insert(&bst_root, "_rm", my_rm);
    bst_insert(&bst_root, "mkdir", my_mkdir);

    while(1) {
        prompt();
        if(getline(&linebuf, &linebuf_size, stdin) < 0) {
            break;
        }

        //将命令添加到历史记录
        add_to_history(linebuf);

        char *line_ori;
        line_ori = (char*)malloc(strlen(linebuf)*sizeof(char));
        strcpy(line_ori, (const char*)linebuf);
        parse(linebuf, &cmd);

        if(strcmp(cmd.globres.gl_pathv[0], "exit") == 0) {
            exit(0);
        }

        // 处理显示历史记录的命令 
        if (strcmp(cmd.globres.gl_pathv[0], "history") == 0) { 
        	display_history(); 
        	continue; // 继续下一次循环，不执行后续的命令处理步骤 
        }

        if(cmd.background == 1) {
            pid = fork();
            if(pid == 0) {
                // Child process
                execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
                perror("execvp()");
                exit(1);
            } else {
                printf("Running in background, PID: %d\n", pid);
            }
        }else {

            int saved_stdout, saved_stdin;
            
            if (handle_pipe(line_ori,bst_root,pid, &saved_stdout, &saved_stdin)) {
                continue;
            }

            execute_command(cmd,bst_root,pid,&saved_stdout,&saved_stdin);
        }
        
    }
    exit(0);
}

