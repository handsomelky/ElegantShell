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

int insensitiveCompare(const void *a, const void *b) {
    const char *str1 = *(const char**)a;
    const char *str2 = *(const char**)b;
    return strcasecmp(str1, str2);
}

void printTree(const char *basePath, int depth, int isLastInParent, int *dirCount, int *fileCount) {
    DIR *dir = opendir(basePath);
    if (!dir) return;

    char *entries[MAX_ENTRIES];
    int count = 0;

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
        for (int j = 0; j < depth; j++) {
            if (j == depth - 1 && isLastInParent) {
                printf("    "); // 如果是当前子目录的最后一个文件，则需要将前面的树状结构空出来
            } else {
                printf("│   ");
            }
        }

        printf("%s ", i == count - 1 ? "└──" : "├──");

        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, entries[i]);
        struct stat statbuf;
        stat(fullPath, &statbuf);

        // 根据文件类型来决定显示结果的颜色
        if (S_ISDIR(statbuf.st_mode)) {
            printf("%s%s%s\n", COLOR_BLUE, entries[i], COLOR_RESET);
            (*dirCount)++;
            printTree(fullPath, depth + 1, i == count - 1, dirCount, fileCount);
        } else {
            printf("%s%s%s\n", (statbuf.st_mode & S_IXUSR) ? COLOR_GREEN : "", entries[i], COLOR_RESET);
            (*fileCount)++;
        }

        free(entries[i]);
    }
}

int mytree(int argc, char **argv) {
    char *dir;
    if (argc >= 2) {
        dir = argv[1];
    } else {
        dir = ".";
    }

    // 统计目录和文件的个数
    int dirCount = 0, fileCount = 0;
    printf("%s%s%s\n", COLOR_BLUE, dir, COLOR_RESET);
    printTree(dir, 0, 0, &dirCount, &fileCount);
    printf("\n%s%d directories, %d files%s\n", COLOR_ORANGE, dirCount, fileCount, COLOR_RESET);
    return 0;
}
