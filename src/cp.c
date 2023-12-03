




// void print_help() {
//     printf("Options:\n");
//     printf("  -d: Create a hard link to the source file\n");
//     printf("  -r, --recursive: Copy directories recursively\n");
//     printf("  -i, --interactive: Prompt before overwrite\n");
//     printf("  -f, --force: Force copy, do not prompt for confirmation\n");
//     printf("  -v, --verbose: Display detailed copy process\n");
//     printf("  -l: Create a hard link to the source file (do not copy)\n");
//     printf("  -b: Backup overwritten files or directories\n");
// }



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <dirent.h>

#define MAX_PATH_LEN 4096
#define BUFSIZE 4096

// 函数声明
void copy_file(const char *src_path, const char *dest_path, int force, int interactive, int verbose, int backup);
void copy_directory(const char *src_path, const char *dest_path, int force, int recursive, int interactive, int verbose, int backup);

// 自定义cp命令主函数
int my_cp(int argc, char *argv[]) {
    // 检查命令行参数数量
    if (argc < 3) {
        fprintf(stderr, "用法: %s [-选项] 源文件/目录 目标文件/目录\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 解析命令行选项
    int c;
    int force = 0, interactive = 0, recursive = 0, verbose = 0, link_ = 0, backup = 0, copy_symlink = 1;

    while ((c = getopt(argc, argv, "drifvlb")) != -1) {
        switch (c) {
            case 'd':
                link_ = 1;
                break;
            case 'r':
                recursive = 1;
                break;
            case 'i':
                interactive = 1;
                break;
            case 'f':
                force = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'l':
                link_ = 1;
                copy_symlink = 0;
                break;
            case 'b':
                backup = 1;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    // 确定源路径和目标路径
    const char *src_path = argv[optind];
    const char *dest_path = argv[optind + 1];

    if (link_) {
        // 创建硬链接
        if (link(src_path, dest_path) != 0) {
            perror("创建硬链接失败");
            exit(EXIT_FAILURE);
        }
    } else {
        struct stat src_stat;
        if (lstat(src_path, &src_stat) == -1) {
            perror("获取文件信息失败");
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(src_stat.st_mode)) {
            // 复制目录
            copy_directory(src_path, dest_path, force, recursive, interactive, verbose, backup);
        } else if (S_ISREG(src_stat.st_mode) || (S_ISLNK(src_stat.st_mode) && copy_symlink)) {
            // 复制普通文件或符号链接
            copy_file(src_path, dest_path, force, interactive, verbose, backup);
        } else {
            fprintf(stderr, "不支持的文件类型\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Copy successful. \n");
    return 0;
}

// 复制普通文件函数
void copy_file(const char *src_path, const char *dest_path, int force, int interactive, int verbose, int backup) {
    struct stat src_stat, dest_stat;

    // 检查目标文件是否存在
    if (lstat(dest_path, &dest_stat) == 0) {
        // 目标文件存在
        if (S_ISDIR(dest_stat.st_mode)) {
            // 目标路径是目录
            // 在目录路径后添加源文件的名称，形成新的目标路径
            char dest_child_path[BUFSIZE];
            snprintf(dest_child_path, BUFSIZE, "%s/%s", dest_path, strrchr(src_path, '/') + 1);

            copy_file(src_path, dest_child_path, force, interactive, verbose, backup);
            return;
        } else if (S_ISREG(dest_stat.st_mode)) {
            // 目标路径是文件，进行正常的复制操作
            if (interactive) {
                // 提示是否覆盖
                char response;
                printf("是否覆盖文件 '%s'? (y/n): ", dest_path);
                scanf(" %c", &response);

                if (response != 'y') {
                    printf("文件未复制。\n");
                    return;
                }
            } else if (!force) {
                // 没有使用-f选项，不允许不经确认就覆盖
                if (!backup) {
                    printf("文件 '%s' 已存在。使用 -f 选项来强制复制。\n", dest_path);
                    return;
                }
            }

            if (backup) {
                // 备份已存在的文件
                char backup_path[BUFSIZE];
                snprintf(backup_path, BUFSIZE, "%s.bak", dest_path);

                if (rename(dest_path, backup_path) == -1) {
                    perror("创建备份失败");
                    exit(EXIT_FAILURE);
                }
                if (verbose) {
                    printf("备份已创建：%s\n", backup_path);
                }
            }
        } else {
            fprintf(stderr, "不支持的文件类型\n");
            exit(EXIT_FAILURE);
        }
    }

    FILE *src_fp = fopen(src_path, "rb");

    if (src_fp == NULL) {
        perror("打开源文件失败");
        exit(EXIT_FAILURE);
    }

    // 检查文件是否为空
    fseek(src_fp, 0, SEEK_END);
    if (ftell(src_fp) == 0) {
        printf("源文件为空。\n");
        fclose(src_fp);
        exit(EXIT_FAILURE);
    }
    rewind(src_fp);
    if (src_fp == NULL) {
        perror("打开源文件失败");
        exit(EXIT_FAILURE);
    }

    FILE *dest_fp = fopen(dest_path, "wb");
    if (dest_fp == NULL) {
        perror("打开目标文件失败");
        fclose(src_fp);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead, bytesWritten;
    char buffer[BUFSIZE];

    // 逐块复制文件内容
    while ((bytesRead = fread(buffer, 1, BUFSIZE, src_fp)) > 0) {
        bytesWritten = fwrite(buffer, 1, bytesRead, dest_fp);
        if (bytesRead != bytesWritten) {
            perror("写入目标文件失败");
            fclose(src_fp);
            fclose(dest_fp);
            exit(EXIT_FAILURE);
        }
    }

    fclose(src_fp);
    fclose(dest_fp);

    if (verbose) {
        printf("文件复制成功：%s -> %s\n", src_path, dest_path);
    }
}

// 复制目录函数
void copy_directory(const char *src_path, const char *dest_path, int force, int recursive, int interactive, int verbose, int backup) {
    // 检查是否递归复制
    if (recursive) {
        struct stat src_stat, des_stat;

        // 获取源目录信息
        if (lstat(src_path, &src_stat) == -1) {
            perror("获取源目录信息失败");
            exit(EXIT_FAILURE);
        }

        // 检查是否是目录
        if (!S_ISDIR(src_stat.st_mode)) {
            fprintf(stderr, "错误: 源路径 '%s' 不是一个目录。\n", src_path);
            exit(EXIT_FAILURE);
        }

        // 打开源目录
        DIR *src_dir = opendir(src_path);
        if (!src_dir) {
            perror("打开源目录失败");
            exit(EXIT_FAILURE);
        }

        char dest_dir_path[BUFSIZE];
        
        // 检查目标路径是否存在
        if (lstat(dest_path, &des_stat) == -1) {
            // 目标路径不存在
            if (strlen(dest_path) < BUFSIZE - 1) {
                strcpy(dest_dir_path, dest_path);
                // 现在 dest_dir_path 包含了 dest_path 的内容
            } else {
                printf("目标路径太长\n");
            }
        } else {
            // 目标路径已存在，构造新的目标路径
            const char *last_slash = strrchr(src_path, '/');
            if (last_slash != NULL) {
                snprintf(dest_dir_path, BUFSIZE, "%s/%s", dest_path, last_slash + 1);
            } else {
                snprintf(dest_dir_path, BUFSIZE, "%s/%s", dest_path, src_path);
            }
        }

        // 创建目标目录
        mkdir(dest_dir_path, 0777);

        // 读取源目录的文件项
        struct dirent *entry;
        while ((entry = readdir(src_dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue; // 跳过 "." 和 ".."
            }

            char src_child_path[BUFSIZE];
            char dest_child_path[BUFSIZE];

            // 构造子文件的源路径和目标路径
            snprintf(src_child_path, BUFSIZE, "%s/%s", src_path, entry->d_name);
            snprintf(dest_child_path, BUFSIZE + 300, "%s/%s", dest_dir_path, entry->d_name);

            struct stat child_stat;

            // 获取子文件信息
            if (lstat(src_child_path, &child_stat) == -1) {
                perror("获取子文件信息失败");
                closedir(src_dir);
                exit(EXIT_FAILURE);
            }

            if (S_ISDIR(child_stat.st_mode)) {
                // 递归复制子目录
                copy_directory(src_child_path, dest_child_path, force, recursive, interactive, verbose, backup);
            } else if (S_ISREG(child_stat.st_mode)) {
                // 复制普通文件
                copy_file(src_child_path, dest_child_path, force, interactive, verbose, backup);
            } else if (S_ISLNK(child_stat.st_mode)) {
                // 复制符号链接
                char link_target[BUFSIZE];
                ssize_t len = readlink(src_child_path, link_target, sizeof(link_target) - 1);
                if (len == -1) {
                    perror("读取链接目标失败");
                    closedir(src_dir);
                    exit(EXIT_FAILURE);
                }
                link_target[len] = '\0';

                if (backup) {
                    // 备份已存在的符号链接
                    char backup_path[BUFSIZE];
                    snprintf(backup_path, BUFSIZE + 4, "%s.bak", dest_child_path);

                    if (rename(dest_child_path, backup_path) == -1) {
                        perror("创建备份失败");
                        closedir(src_dir);
                        exit(EXIT_FAILURE);
                    }
                    if (verbose) {
                        printf("备份已创建：%s\n", backup_path);
                    }
                }

                if (interactive) {
                    // 提示是否覆盖符号链接
                    char response;
                    printf("是否覆盖符号链接 '%s'? (y/n): ", dest_child_path);
                    scanf(" %c", &response);

                    if (response != 'y') {
                        printf("符号链接未复制。\n");
                        continue;
                    }
                } else if (!force) {
                    // 没有使用 -f 选项，不允许不经确认就覆盖
                    printf("符号链接 '%s' 已存在。使用 -f 选项来强制复制。\n", dest_child_path);
                    continue;
                }

                // 创建新的符号链接
                if (symlink(link_target, dest_child_path) == -1) {
                    perror("创建符号链接失败");
                    closedir(src_dir);
                    exit(EXIT_FAILURE);
                }
                if (verbose) {
                    printf("符号链接复制成功：%s -> %s\n", src_child_path, dest_child_path);
                }
            } else {
                fprintf(stderr, "不支持的文件类型\n");
                closedir(src_dir);
                exit(EXIT_FAILURE);
            }
        }

        closedir(src_dir);

        if (verbose) {
            printf("目录复制成功：%s -> %s\n", src_path, dest_path);
        }
    } else {
        fprintf(stderr, "错误：未指定 -r 选项；\n");
        exit(EXIT_FAILURE);
    }
}
