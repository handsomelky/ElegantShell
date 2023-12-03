#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_ENTRIES 1024

// 定义ANSI颜色代码
#define COLOR_BLUE "\x1b[34m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"
#define COLOR_ORANGE "\x1b[33m"
#define COLOR_RED "\x1b[31m"
#define COLOR_MAGENTA "\x1b[35m"



int insensitiveCompare(const void *a, const void *b) {
    const char *str1 = *(const char**)a;
    const char *str2 = *(const char**)b;
    return strcasecmp(str1, str2);
}

// 检查文件是否为压缩包
int isCompressed(const char *filename) {
    const char *ext = strrchr(filename, '.');
    return ext != NULL && (strcmp(ext, ".zip") == 0 || strcmp(ext, ".rar") == 0);
}

void printTree(const char *basePath, int depth, int isLast[], int *dirCount, int *fileCount, int showHidden, int dirsOnly, int maxDepth) {
    DIR *dir = opendir(basePath);
    if (!dir) return;

    char *entries[MAX_ENTRIES];
    int count = 0;

    // 判断目录层级
    if (maxDepth >= 0 && depth > maxDepth-1) {
        return;
    }

    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            entries[count] = strdup(dp->d_name);
            count++;
        }
    }

    closedir(dir);
    // 对文件名的首字母进行排序
    qsort(entries, count, sizeof(char*), insensitiveCompare);

    for (int i = 0; i < count; i++) {

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, entries[i]);
        struct stat statbuf;
        stat(fullPath, &statbuf);
        // 当'-d'被设定时，会跳过文件的显示

        if (dirsOnly && !S_ISDIR(statbuf.st_mode)) {
            continue;
        }

        if (!showHidden && entries[i][0] == '.') {
            continue;
        }
        for (int j = 0; j < depth; j++) {
            printf(isLast[j] ? "    " : "│   ");// 如果是当前子目录的最后一个文件，则需要将前面的树状结构空出来
        }

        printf("%s ", i == count - 1 ? "└──" : "├──");


        // 决定是否显示隐藏文件


         // 根据文件类型来决定显示结果的颜色
        if (S_ISDIR(statbuf.st_mode)) {
            printf("%s%s%s\n", COLOR_BLUE, entries[i], COLOR_RESET);
            (*dirCount)++;
            isLast[depth] = (i == count - 1);
            printTree(fullPath, depth + 1, isLast, dirCount, fileCount,showHidden,dirsOnly, maxDepth);
        }
        else {
            
            if(entries[i][0] == '.'){
                printf("%s%s%s\n", COLOR_MAGENTA, entries[i], COLOR_RESET);
            }else if (isCompressed(entries[i])) {
                printf("%s%s%s\n", COLOR_RED, entries[i], COLOR_RESET);
            }else{
                printf("%s%s%s\n", (statbuf.st_mode & S_IXUSR) ? COLOR_GREEN : "", entries[i], COLOR_RESET);
            }
            
            (*fileCount)++;
        }

        free(entries[i]);
    }
}


int mytree(int argc, char **argv) {
    char *dir=".";
    int showHidden = 0,  dirsOnly = 0, maxDepth = -1;
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') { 
            if (strcmp(argv[i], "-a") == 0) {
                showHidden = 1;
            } else if (strcmp(argv[i], "-d") == 0) {
                dirsOnly = 1;
            } else if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
                maxDepth = atoi(argv[++i]);
                if (maxDepth <= 0) {
                    fprintf(stderr, "tree: Invalid level, must be greater than 0.\n");
                    return -1;
                }
            }
        } else {
            dir = argv[i];
            break;
        }
    }

    // 统计目录和文件的个数
    int dirCount = 0, fileCount = 0;
    int isLast[MAX_ENTRIES] = {0};
    printf("%s%s%s\n", COLOR_BLUE, dir, COLOR_RESET);
    printTree(dir, 0, isLast, &dirCount, &fileCount,showHidden,dirsOnly, maxDepth);
    if (dirsOnly) {
        printf("\n%s%d directories%s\n", COLOR_ORANGE, dirCount, COLOR_RESET);
    } else {
        printf("\n%s%d directories, %d files%s\n", COLOR_ORANGE, dirCount, fileCount, COLOR_RESET);
    }    
    return 0;
}

