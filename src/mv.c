// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <limits.h>
// #include <libgen.h>

// #define BUF_SIZE 4096

// int my_mv(int argc, char **argv) {
//     // 检查参数个数
//     if (argc != 3) {
//         fprintf(stderr, "mv: 用法错误\n");
//         return 1;
//     }

//     const char *src = argv[1];
//     const char *dest = argv[2];

//     struct stat st;

//     // 获取目标路径的信息
//     if (stat(dest, &st) == 0) {
//         if (S_ISDIR(st.st_mode)) {
//             // 如果目标是目录，生成新的文件名
//             char dest_path[PATH_MAX];

//             // 在函数开始处定义一个可写的字符串
//             char src_copy[PATH_MAX];
//             strncpy(src_copy, src, PATH_MAX - 1);
//             src_copy[PATH_MAX - 1] = '\0';  // 确保以 null 结尾

//             // 获取文件名（不包含路径）和文件格式后缀
//         char *file_name = basename(src_copy);
//         char *file_extension = strrchr(file_name, '.');
//         if (file_extension != NULL) {
//             *file_extension = '\0';  // 从最后一个点开始分隔文件名和文件格式后缀
//             file_extension++;        // 移动到文件格式后缀的起始位置
//         }

//             // 使用 snprintf 生成新的文件路径
//             snprintf(dest_path, PATH_MAX, "%s/%s.%s", dest, file_name, file_extension);

//             // 检查新文件名是否已经存在，如果存在则在文件名末尾添加序号
//             int counter = 1;  // 从下一个序号开始
//             while (access(dest_path, F_OK) == 0 && counter <= 100) {
//                 // 使用临时变量构建新的文件路径
//                 char temp_dest_path[PATH_MAX];
//                 snprintf(temp_dest_path, PATH_MAX, "%s/%s_%d.%s", dest, file_name, counter, file_extension);

//                 // 将临时变量的内容赋给 dest_path
//                 strncpy(dest_path, temp_dest_path, PATH_MAX - 1);
//                 dest_path[PATH_MAX - 1] = '\0';  // 确保以 null 结尾

//                 counter++;
//             }

//             if (counter > 100) {
//                 fprintf(stderr, "mv: 无法为 '%s' 生成唯一的文件名\n", dest);
//                 return 1;
//             }

//             // 执行文件移动
//             if (rename(src, dest_path) != 0) {
//                 perror("rename()");
//                 return 1;
//             }
//         } else {
//             // 如果目标是文件，直接执行文件移动
//             if (rename(src, dest) != 0) {
//                 perror("rename()");
//                 return 1;
//             }
//         }
//     } else {
//         // 如果目标路径不存在，尝试创建目录
//         if (mkdir(dest, 0777) == -1) {
//             perror("Error creating destination directory");
//             return 1;
//         }

//         // 然后继续生成新的文件名并执行文件移动
//         char dest_path[PATH_MAX];
//         char src_copy[PATH_MAX];
//         strncpy(src_copy, src, PATH_MAX - 1);
//         src_copy[PATH_MAX - 1] = '\0';  // 确保以 null 结尾

//          // 获取文件名（不包含路径）和文件格式后缀
//         char *file_name = basename(src_copy);
//         char *file_extension = strrchr(file_name, '.');
//         if (file_extension != NULL) {
//             *file_extension = '\0';  // 从最后一个点开始分隔文件名和文件格式后缀
//             file_extension++;        // 移动到文件格式后缀的起始位置
//         }

//         snprintf(dest_path, PATH_MAX, "%s/%s.%s", dest, file_name, file_extension);

//         int counter = 1;  // 从下一个序号开始
//         while (access(dest_path, F_OK) == 0 && counter <= 100) {
//             char temp_dest_path[PATH_MAX];
//             snprintf(temp_dest_path, PATH_MAX, "%s/%s_%d.%s", dest, file_name, counter, file_extension);
//             strncpy(dest_path, temp_dest_path, PATH_MAX - 1);
//             dest_path[PATH_MAX - 1] = '\0';  // 确保以 null 结尾
//             counter++;
//         }

//         if (counter > 100) {
//             fprintf(stderr, "mv: 无法为 '%s' 生成唯一的文件名\n", dest);
//             return 1;
//         }

//         // 执行文件移动
//         if (rename(src, dest_path) != 0) {
//             perror("rename()");
//             return 1;
//         }
//     }

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <getopt.h>

#define PATH_MAX 4096

void move(const char *source, const char *destination,int backup,int interactive,int force,int no_clobber);
void make_backup(const char *file_path) {
    char backup_path[PATH_MAX];
    snprintf(backup_path, sizeof(backup_path), "%s~", file_path);

    // 如果备份文件已经存在，则添加序号
    int counter = 1;
    while (access(backup_path, F_OK) != -1) {
        snprintf(backup_path, sizeof(backup_path), "%s~%d", file_path, counter);
        counter++;
    }

    // 复制文件或目录到备份路径
    if (link(file_path, backup_path) == -1) {
        perror("Error creating backup");
        return;
    }

    printf("Backup created: %s\n", backup_path);
}

int my_mv(int argc, char *argv[]) {
    int opt;
    int backup = 0;
    int interactive = 0;
    int force = 0;
    int no_clobber = 0;
    
    while ((opt = getopt(argc, argv, "bifn")) != -1) {
        switch (opt) {
            case 'b':
                backup = 1;
                break;
            case 'i':
                interactive = 1;
                break;
            case 'f':
                force = 1;
                break;
            case 'n':
                no_clobber = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-bifnu] <source> <destination>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // 剩余的命令行参数是源和目标路径
    if (optind + 2 != argc) {
        fprintf(stderr, "Usage: %s [-fir] <source> <destination>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *source = argv[optind];
    const char *destination = argv[optind + 1];

    move(source, destination, backup, interactive, force, no_clobber);

    printf("Move successful.\n");
    return 0;
}


void move(const char *source, const char *destination,int backup,int interactive,int force,int no_clobber) {
    struct stat source_stat, dest_stat;
    
    // 获取源文件或目录信息
    if (stat(source, &source_stat) == -1) {
        perror("Error getting source file/directory information");
        return;
    }

    // 获取目标文件或目录信息
    if (stat(destination, &dest_stat) == -1) {
        if (rename(source, destination) == -1) {
            perror("Error moving");
        }
        return;
    }
    if (backup && no_clobber) {
        fprintf(stderr, "mv: options --backup and --no-clobber are mutually exclusive\n");
        exit(EXIT_FAILURE);
    }
    // 处理 -n 选项
    if (no_clobber && S_ISREG(dest_stat.st_mode)) {
        fprintf(stderr, "Error: Destination file exists and -n specified.\n");
        return;
    }

    // 如果源是文件，目标是文件，进行重命名
    if (S_ISREG(source_stat.st_mode) && S_ISREG(dest_stat.st_mode)) {
        if (interactive) {
        printf("File '%s' already exists. Overwrite? (y/n): ", destination);
        char response;
        scanf(" %c", &response);
        if (response != 'y') {
            printf("Move operation canceled.\n");
            return;
        }
    }

    // 如果开启备份，先创建备份
    if (backup) {
        make_backup(destination);
    }

    if (rename(source, destination) == -1) {
        perror("Error moving");
        return;
    }
    }
    // 如果源是文件，目标是目录，移动文件到目录中
    else if (S_ISREG(source_stat.st_mode) && S_ISDIR(dest_stat.st_mode)) {
        if (interactive) {
            printf("File '%s' already exists. Overwrite? (y/n): ", destination);
            char response;
            scanf(" %c", &response);
            if (response != 'y') {
                printf("Move operation canceled.\n");
                return;
            }
        }

        char dest_path[PATH_MAX];
        snprintf(dest_path, PATH_MAX, "%s/%s", destination, source);

        // 如果开启备份，先创建备份
        if (backup) {
            // 创建备份的逻辑
            make_backup(destination);
        }

        if (rename(source, dest_path) == -1) {
            perror("Error moving");
            return;
        }
    }

    else if (S_ISDIR(source_stat.st_mode) && S_ISDIR(dest_stat.st_mode)) {
        // 创建目标目录
    // 构建目标目录的完整路径
    char dest_path[PATH_MAX];
    char dest_entry_path[PATH_MAX];

    if (snprintf(dest_path, PATH_MAX, "%s/%s", destination, source) >= PATH_MAX) {
        fprintf(stderr, "Destination path too long\n");
        return;
    }

    // 创建目标目录
    if (mkdir(dest_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        perror("Error creating destination directory");
        return;
    }

    // 打开源目录
    DIR *dir = opendir(source);
    if (dir == NULL) {
        perror("Error opening source directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // 构建源和目标的完整路径
            if (snprintf(dest_entry_path, PATH_MAX, "%s/%s", dest_path, entry->d_name) >= PATH_MAX) {
                fprintf(stderr, "Destination path too long\n");
                closedir(dir);
                return;
            }

            char source_path[PATH_MAX];
            snprintf(source_path, PATH_MAX, "%s/%s", source, entry->d_name);

            // 移动文件（使用 rename 或者复制后删除）
            if (rename(source_path, dest_entry_path) == -1) {
                // 处理移动失败的情况
                perror("Error moving file");
            }
        }
    }

    // 关闭源目录
    closedir(dir);

    // 删除源目录
    if (rmdir(source) == -1) {
        perror("Error removing source directory");
        return;
    }
    }
    // 如果源是目录，目标是文件，出错
    else if (S_ISDIR(source_stat.st_mode) && S_ISREG(dest_stat.st_mode)) {
        fprintf(stderr, "Error: Cannot move directory to file\n");
        return;
    }
    // 其他情况，报错
    else {
        fprintf(stderr, "Unsupported file type\n");
        return;
    }
 }

