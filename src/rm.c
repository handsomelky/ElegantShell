// void print_help() {
//     printf("Options:\n");
//     printf("  -f: Force deletion without confirmation\n");
//     printf("  -i: Interactive mode, prompt before each deletion\n");
//     printf("  -r: Recursive deletion, including subdirectories and their contents\n");
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wchar.h>
#include <locale.h>
#include <bits/getopt_core.h>

#define BUFFER_SIZE 256
#define PATH_MAX 4096

// 清空输入缓冲区的函数
void clear_input_buffer_rm() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 删除目录的函数
void remove_directory(char *dir_path, int force, int interactive) {
    if (!force && interactive) {
        char response[BUFFER_SIZE];
        printf("删除目录 %s 及其内容? (y/n): ", dir_path);
        fgets(response, sizeof(response), stdin);

        if (response[0] != 'y' && response[0] != 'Y') {
            return;
        }
    }

    if (rmdir(dir_path) == -1) {
        perror("删除目录出错");
    }
}

// 删除文件的函数
void remove_file(char *file_path, int force, int interactive) {
    if (!force && interactive) {
        char response[BUFFER_SIZE];
        printf("删除文件 %s? (y/n): ", file_path);
        fgets(response, sizeof(response), stdin);

        if (response[0] != 'y' && response[0] != 'Y') {
            return;
        }
    }

    if (unlink(file_path) == -1) {
        perror("删除文件出错");
    }
}

// 递归删除目录及其内容的函数
void remove_recursive(char *path, int force, int interactive) {
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (!dir) {
        perror("打开目录出错");
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char entry_path[PATH_MAX];
            snprintf(entry_path, PATH_MAX, "%s/%s", path, entry->d_name);

            if (entry->d_type == DT_DIR) {
                remove_recursive(entry_path, force, interactive);  // 递归删除子目录
            } else {
                remove_file(entry_path, force, interactive);  // 删除文件
            }
        }
    }

    closedir(dir);
    remove_directory(path, force, interactive);  // 删除当前目录及其内容后删除
}

// 主删除函数
int my_rm(int argc, char *argv[]) {
    int force = 0;
    int recursive = 0;
    int interactive = 0;
    optind = 1;
    int opt;

    // 解析命令行参数
    while ((opt = getopt(argc, argv, "fir")) != -1) {
        switch (opt) {
            case 'f':
                force = 1;
                break;
            case 'i':
                interactive = 1;
                break;
            case 'r':
                recursive = 1;
                break;
            default:
                fprintf(stderr, "rm: 用法: %s [-fir] <文件1> [文件2] [文件3] ...\n", argv[0]);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "用法: %s [-fir] <文件1> [文件2] [文件3] ...\n", argv[0]);
    }

    // 遍历命令行参数中的文件或目录
    for (int i = optind; i < argc; i++) {
        char *path = argv[i];

        struct stat path_stat;
        if (stat(path, &path_stat) == -1) {
            perror("获取文件/目录信息出错");
        }

        // 根据选项执行删除操作
        if (recursive && S_ISDIR(path_stat.st_mode)) {
            remove_recursive(path, force, interactive);
        } else if (force || (interactive && S_ISREG(path_stat.st_mode))) {
            remove_file(path, force, interactive);
        } else {
            // 对于非选项的情况，直接删除文件或目录
            if (argc - optind == 1) {
                if (S_ISDIR(path_stat.st_mode)) {
                    remove_recursive(path, force, interactive);
                } else if (S_ISREG(path_stat.st_mode)) {
                    remove_file(path, force, interactive);
                } else {
                    fprintf(stderr, "对于 %s，选项的组合无效\n", path);
                }
            } else {
                fprintf(stderr, "对于 %s，选项的组合无效\n", path);
            }
        }
    }
    printf("Remove successful.\n");
    return 0;
}
