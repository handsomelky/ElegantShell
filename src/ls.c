#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <locale.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "ls.h"

// 定义ANSI颜色代码
#define COLOR_BLUE "\x1b[34m"
#define COLOR_GREEN "\x1b[32m" 
#define COLOR_RESET "\x1b[0m"

#define Path_MAX 1024

void listFiles(const char *dirPath, int showHidden, int showDetails,int sortByTime);
void printPermissions(mode_t mode); 
void colour(mode_t mode, char *name);
int letterCompare(const struct dirent **a, const struct dirent **b);
int compareTime(const void *A, const void *B);

int ls(int argc, char **argv) {
    int dir_num = 0;         // 目录在argv中位置
    int showHidden = 0;      // 是否显示隐藏文件
    int showDetails = 0;     // 是否显示详细信息
    int sortByTime = 0;      // 是否按时间排序
    char dir_path[Path_MAX];
    if (argc < 1 || argc > 4) {
        fprintf(stderr, "Usage: %s [-a] [-l] [-t] [directory]\n", argv[0]);
        return 1;
    } else if (argc == 1) {                     // ls
        strcpy(dir_path, ".");
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-') {
                for (int j = 1; j < strlen(argv[i]); j++) {
                    if (argv[i][j] == 'a') {
                        showHidden = 1; 
                    } else if (argv[i][j] == 'l') {
                        showDetails = 1;
                    } else if (argv[i][j] == 't') {
                        sortByTime = 1;
                    } else {
                        fprintf(stderr, "Usage: %s [-a] [-l] [-t] [directory]\n", argv[0]);
                        return -1;
                    }
                }
            } else {
                dir_num = i;
            }
        }
    }
    if (dir_num == 0)
        strcpy(dir_path, ".");
    else
        strcpy(dir_path, argv[dir_num]);
    listFiles(dir_path, showHidden, showDetails,sortByTime);
    return 0;
}

void listFiles(const char *dirPath, int showHidden, int showDetails,int sortByTime){ 
    struct dirent **fileList;
    int numFiles = scandir(dirPath, &fileList, NULL, letterCompare);
    if (numFiles < 0) 
        fprintf(stderr, "'cd':No such file or directory");
    if(sortByTime)
        qsort(fileList, numFiles, sizeof(struct dirent *), compareTime);
    for (int i = 0; i < numFiles; i++) {
        char filePath[Path_MAX];
        sprintf(filePath, "%s/%s", dirPath, fileList[i]->d_name);
        struct stat fileStat;
        stat(filePath, &fileStat);
        if (!showHidden && fileList[i]->d_name[0] == '.')
            continue;
        if (!showDetails)
            colour(fileStat.st_mode, fileList[i]->d_name);
        else {
            
            struct stat fileStat;
            struct passwd *user;
            struct group *group;
            struct tm *timeInfo;
            stat(fileList[i]->d_name, &fileStat);
            user = getpwuid(fileStat.st_uid);
            group = getgrgid(fileStat.st_gid);
            timeInfo = localtime(&fileStat.st_mtime);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%b %d %H:%M", timeInfo);
            printPermissions(fileStat.st_mode);
            printf(" %-2ld", (long)fileStat.st_nlink);
            printf("%-6s", user->pw_name);
            printf("%-6s", group->gr_name);
            printf("%-6ld", (long)fileStat.st_size);
            printf("%-13s", buffer);
            colour(fileStat.st_mode, fileList[i]->d_name);
            printf("\n"); 
        }
    }
    if(!showDetails)
        printf("\n");
    for (int i = 0; i < numFiles; i++) {
        free(fileList[i]);
    }
    free(fileList);
}

void printPermissions(mode_t mode) {          //文件权限
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void colour(mode_t mode, char *name) {       //选择文件颜色
    if (S_ISDIR(mode)) {
        printf("%s%s%s  ", COLOR_BLUE, name, COLOR_RESET);
    } else {
        printf("%s%s%s  ", (mode & S_IXUSR) ? COLOR_GREEN : "", name, COLOR_RESET);
    }
}

int compareTime(const void *A, const void *B) {     //按时间排序
    const struct dirent *a = *(const struct dirent **)A;
    const struct dirent *b = *(const struct dirent **)B;
    struct stat statA, statB;
    stat(a->d_name, &statA);
    stat(b->d_name, &statB);
    return statB.st_mtime - statA.st_mtime;
}

int letterCompare(const struct dirent **a, const struct dirent **b) {  //按首字母排序
    return strcasecmp((*a)->d_name, (*b)->d_name);
}
