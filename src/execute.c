#include "execute.h" 
#include "main.h" 

void execute_command(cmd_st cmd, BST_NODE *bst_root,pid_t pid, int *saved_stdout, int *saved_stdin) {
    // Insert the command execution code here

    handle_io_redirection(&cmd.globres.gl_pathc, cmd.globres.gl_pathv, &saved_stdout, &saved_stdin);

    BST_NODE *np = bst_search(bst_root, cmd.globres.gl_pathv[0]);
    if (np != NULL) {
        np->handler(cmd.globres.gl_pathc, cmd.globres.gl_pathv);

    } else {
        pid = fork();
        if(pid < 0) {
            perror("fork()");
            exit(1);
        }
        if(pid == 0) {
            execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
            perror("execvp()");
            exit(1);
        } else {
            wait(NULL);
        }

    }
    fflush(stdout);
    // 恢复原始的标准输出和输入
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdout);
    close(saved_stdin);

    puts("");

    
}
