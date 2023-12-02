
#include <stdio.h>
#include <string.h>

void print_help_cd() {
    printf("Usage: cd [directory]\n");
    printf("Change the current working directory.\n");
    printf("Options:\n");
    printf("  No options are supported.\n");
    printf("Examples:\n");
    printf("  cd            # Change to the home directory\n");
    printf("  cd ~          # Equivalent to 'cd' (change to the home directory)\n");
    printf("  cd my_folder  # Change to the 'my_folder' directory\n");
}

void print_help_clear() {
    printf("Usage: clear\n");
    printf("Clear the terminal screen.\n");
    printf("Options:\n");
    printf("  No options are supported.\n");
    printf("Examples:\n");
    printf("  clear\n");
}

void print_help_cp() {
    printf("Usage: cp [OPTIONS] SOURCE DESTINATION\n");
    printf("Copy SOURCE to DESTINATION.\n\n");

    printf("Options:\n");
    printf("  -d: Create a hard link to the source file\n");
    printf("  -r, --recursive: Copy directories recursively\n");
    printf("  -i, --interactive: Prompt before overwrite\n");
    printf("  -f, --force: Force copy, do not prompt for confirmation\n");
    printf("  -v, --verbose: Display detailed copy process\n");
    printf("  -l: Create a hard link to the source file (do not copy)\n");
    printf("  -b: Backup overwritten files or directories\n");
}

void print_help_history() {
    printf("Usage: display_history\n");
    printf("Display the command history in reverse chronological order (newest to oldest).\n");
    printf("Options:\n");
    printf("  No options are supported.\n");
}

void print_help_io_redirection() {
    printf("Usage: handle_io_redirection [OPTIONS] ARGC ARGV SAVED_STDOUT SAVED_STDIN\n");
    printf("Handle input/output redirection in command-line arguments.\n\n");

    printf("Options:\n");
    printf("  No options are supported.\n");
    printf("Arguments:\n");
    printf("  ARGC: Pointer to the size_t variable storing the number of command-line arguments\n");
    printf("  ARGV: Array of command-line arguments\n");
    printf("  SAVED_STDOUT: Pointer to the variable storing the original STDOUT file descriptor\n");
    printf("  SAVED_STDIN: Pointer to the variable storing the original STDIN file descriptor\n\n");

    printf("Description:\n");
    printf("  This function handles input/output redirection in the given command-line arguments.\n");
    printf("  It supports the following redirection operators:\n");
    printf("    >   Redirects standard output to a file (creates or truncates the file)\n");
    printf("    >>  Appends standard output to a file (creates or appends to the file)\n");
    printf("    <   Redirects standard input from a file\n");
    printf("  The function modifies the ARGC and ARGV parameters to reflect any changes due to\n");
    printf("  redirection. It also saves and restores the original STDOUT and STDIN file descriptors.\n");
}

void print_help_ls() {
    printf("Usage: ls [OPTIONS] [DIRECTORY]\n");
    printf("List information about files in the specified directory.\n\n");

    printf("Options:\n");
    printf("  -a: Include entries whose names begin with a dot (.)\n");
    printf("  -l: List in long format, displaying detailed information\n");
    printf("  -t: Sort by modification time, newest first\n");
    printf("  -h: Display help information\n");
    printf("  DIRECTORY: The directory to list (default is the current directory)\n");
}

void print_help_wc() {
    printf("Usage: wc [OPTIONS] [FILES]\n");
    printf("Count lines, words, and characters in files or standard input.\n\n");

    printf("Options:\n");
    printf("  -i: Read input from redirection instead of files\n");
    printf("  -l: Count lines\n");
    printf("  -w: Count words\n");
    printf("  -c: Count characters\n");
    printf("  FILES: The files to count or use '-' for standard input\n");
}

void print_help_mytree() {
    printf("Usage: mytree [OPTIONS] [DIRECTORY]\n");
    printf("List contents of directories in a tree-like format.\n\n");

    printf("Options:\n");
    printf("  -a: Include hidden files and directories\n");
    printf("  -d: List directories only\n");
    printf("  -L <level>: Limit the display to the specified level of the directory tree\n");
    printf("  DIRECTORY: The directory to display (default is the current directory)\n");
}

void print_help_my_mkdir() {
    printf("Usage: my_mkdir [OPTIONS] <directory_name>\n");
    printf("Create one or more directories.\n\n");

    printf("Options:\n");
    printf("  -p: Create parent directories as needed\n");
    printf("  <directory_name>: The name of the directory to create\n");
}

void print_help_my_mv() {
    printf("Usage: my_mv [OPTIONS] <source> <destination>\n");
    printf("Move files or directories.\n\n");

    printf("Options:\n");
    printf("  -b: Create a backup of the destination file before overwriting\n");
    printf("  -i: Prompt before overwriting an existing file\n");
    printf("  -f: Force move, overwrite the destination file without prompting\n");
    printf("  -n: Do not overwrite an existing file (no-clobber)\n");
    printf("  <source>: The file or directory to move\n");
    printf("  <destination>: The destination path for the move operation\n");
}

void print_process_info_help() {
    printf("Usage: ps\n");
    printf("Display information about processes.\n\n");

    printf("Options:\n");
    printf("  No options available\n");
    printf("  Displays process information including PID, status, utime, stime, and vsize.\n");
}

void pwd_help() {
    printf("Usage: pwd\n");
    printf("Print the current working directory.\n\n");

    printf("Options:\n");
    printf("  No options available\n");
    printf("  Displays the absolute pathname of the current working directory.\n");
}

void print_help_my_rm() {
    printf("Usage: my_rm [OPTIONS] <file1> [file2] [file3] ...\n");
    printf("Remove files or directories.\n\n");

    printf("Options:\n");
    printf("  -f: Force removal without confirmation\n");
    printf("  -i: Interactive mode (prompt before removal)\n");
    printf("  -r: Remove directories and their contents recursively\n");
    printf("  <file1> [file2] [file3] ...: Files or directories to remove\n");
}

void print_help_my_cat() {
    printf("Usage: my_cat [OPTIONS] [file1 file2 ...]\n");
    printf("Concatenate and display the content of files.\n\n");

    printf("Options:\n");
    printf("  -n: Number the output lines\n");
    printf("  [file1 file2 ...]: List of files to concatenate and display\n");
}

void print_help_my_echo() {
    printf("Usage: my_echo [OPTIONS] [message]\n");
    printf("Print the specified message to the standard output.\n\n");

    printf("Options:\n");
    printf("  -n: Do not output the trailing newline\n");
    printf("  [message]: The message to be printed\n");
}

void print_help_head() {
printf("Usage: head -n <lines> <file>\n");
printf("Display the first few lines of a file.\n\n");

printf("Options:\n");
printf(" -n <lines>: Specify the number of lines to display\n");
printf(" <file>: The name of the file\n");
}

void print_help_tail() {
printf("Usage: tail -n <lines> <file>\n");
printf("Display the last few lines of a file.\n\n");

printf("Options:\n");
printf(" -n <lines>: Specify the number of lines to display\n");
printf(" <file>: The name of the file\n");
}


void display_help(int argc, char **argv) {
    if (argc >= 2) {
        const char *command = argv[1];
        
        printf("\n");

        if (strcmp(command, "hello") == 0) {
            printf("Usage: hello\n");
            printf("Prints a greeting message.\n");
        } 
		else if (strcmp(command, "tree") == 0) {
            print_help_mytree();
        } 
		else if (strcmp(command, "wc") == 0) {
            print_help_wc();
        } 
		else if (strcmp(command, "ls") == 0) {
            print_help_ls();
        }
        else if (strcmp(command, "cd") == 0) {
            print_help_cd();
        }
        else if (strcmp(command, "clear") == 0) {
            print_help_clear();
        }
        else if (strcmp(command, "_cp") == 0) {
            print_help_cp();
        }
        else if (strcmp(command, "history") == 0) {
            print_help_history();
        }
        else if (strcmp(command, "io_redirection") == 0) {
            print_help_io_redirection();
        }
        else if (strcmp(command, "mkdir") == 0) {
            print_help_my_mkdir(); 
        }
        else if (strcmp(command, "_mv") == 0) {
            print_help_my_mv();
        }
        else if (strcmp(command, "ps") == 0) {
            print_process_info_help(); 
        }
        else if (strcmp(command, "pwd") == 0) {
            pwd_help(); 
        }
        else if (strcmp(command, "_rm") == 0) {
           print_help_my_rm(); 
        }
        else if (strcmp(command, "cat") == 0) {
           print_help_my_cat();
        }
        else if (strcmp(command, "echo") == 0) {
           print_help_my_echo();
        }
        else if (strcmp(command, "head") == 0) {
           print_help_head();
        }
        else if (strcmp(command, "tail") == 0) {
           print_help_tail();
        }
        // Add more else if statements for other commands
        else {
            fprintf(stderr, "Unknown command: %s\n", command);
        }
        
    } else {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
    }
}

