
// void print_help() {
//     printf("Options:\n");
//     printf("  -b: Backup files or directories before overwriting\n");
//     printf("  -i: Prompt before overwriting\n");
//     printf("  -f: Force move, do not prompt for confirmation\n");
//     printf("  -n: Do not overwrite an existing file\n");
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


void move(const char *source, const char *destination,int backup,int interac_mvtive,int force,int no_clobber) {
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
        if (interac_mvtive) {
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
        if (interac_mvtive) {
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

