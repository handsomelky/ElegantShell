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

void listFiles(const char *dirPath, int showHidden, int showDetails);  //分类输出打印

int ls(int argc, char **argv){
    int dir_num = 0;         // 目录在argv中位置
    int showHidden = 0;      // 是否显示隐藏文件
    int showDetails = 0;     // 是否显示详细信息
    char dir_path[Path_MAX];
    if(argc < 1 || argc > 3){
        fprintf(stderr, "help+%s 查询用法\n",argv[0]);
        return 1;
    }
    else if(argc == 1){                     //ls
        strcpy(dir_path, ".");
    }
    else{
        for(int i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                for(int j = 1; j < strlen(argv[i]); j++){
                    if(argv[i][j] == 'a'){
                        showHidden = 1; 
                    }
                    else if(argv[i][j] == 'l'){
                        showDetails = 1;
                    }
                    else{
                        fprintf(stderr, "help+%s 查询用法\n", argv[0]);
                        return -1;
                    }
                }
            }
            else{
                dir_num = i;
            }
        }
    }
    if(dir_num == 0)
        strcpy(dir_path, ".");
    else
        strcpy(dir_path, argv[dir_num]);
    listFiles(dir_path, showHidden, showDetails);
    return 0;
}

void printPermissions(mode_t mode) {
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

void colour(mode_t mode, char *name)      //选择颜色
{
    // 根据文件类型来决定显示结果的颜色
        if (S_ISDIR(mode)) {
            printf("%s%s%s  ", COLOR_BLUE, name, COLOR_RESET);
        } else {
            printf("%s%s%s  ", (mode & S_IXUSR) ? COLOR_GREEN : "", name, COLOR_RESET);
        }
}

void listFiles(const char *dirPath, int showHidden, int showDetails){
    DIR *dir;
    struct dirent *all_dir;
    struct stat fileStat;
    struct passwd *user;
    struct group *group;
    struct tm *timeInfo;

    dir = opendir(dirPath);
    if(dir == NULL){
        fprintf(stderr, "'cd':No such file or directory");
        return;
    }

    while((all_dir = readdir(dir)) != NULL){
        stat(all_dir->d_name, &fileStat);
        if(!showHidden && all_dir->d_name[0] == '.')
            continue;
        if(!showDetails)               
            colour(fileStat.st_mode, all_dir->d_name);
        else{                                            
            user = getpwuid(fileStat.st_uid);
            group = getgrgid(fileStat.st_gid);
            timeInfo = localtime(&fileStat.st_mtime);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%b %d %H:%M", timeInfo);
            printPermissions(fileStat.st_mode);
            printf("%ld  ", (long)fileStat.st_nlink);
            printf("%s  ", user->pw_name);
            printf("%s  ", group->gr_name);
            printf("%ld  ", (long)fileStat.st_size);
            printf("%s  ", buffer);
            colour(fileStat.st_mode, all_dir->d_name);
            printf("\n");
        }
    }
    closedir(dir);
}


