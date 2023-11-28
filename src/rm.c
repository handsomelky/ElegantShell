// #define _GNU_SOURCE
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <dirent.h>
// #include <string.h>
// #include <limits.h>
// #include <bits/getopt_core.h>

// #define PATH_MAX 4096
// int interactive = 0;

// void remove_file(char *file_path) {
//     printf("remove_file\n");
//     if (interactive) {
//         char response;
//         printf("Remove file %s? (y/n): ", file_path);
//         scanf(" %c", &response);
//         if (response != 'y') {
//             printf("Skipped: %s\n", file_path);
//             return;
//         }
//     }

//     if (unlink(file_path) == -1) {
//         perror("Error removing file");
//         exit(EXIT_FAILURE);
//     }
// }

// void remove_directory(char *dir_path) {
//     printf("remove_directory\n");
//     if (interactive) {
//         char response;
//         printf("Remove directory %s and its contents? (y/n): ", dir_path);
//         scanf(" %c", &response);
//         if (response != 'y') {
//             printf("Skipped: %s\n", dir_path);
//             return;
//         }
//     }

//     if (rmdir(dir_path) == -1) {
//         perror("Error removing directory");
//         exit(EXIT_FAILURE);
//     }
// }

// void remove_recursive(char *path) {
//     printf("remove_recursive\n");
//     DIR *dir = opendir(path);
//     struct dirent *entry;

//     if (!dir) {
//         perror("Error opening directory");
//         exit(EXIT_FAILURE);
//     }

//     while ((entry = readdir(dir)) != NULL) {
//         if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//             char entry_path[PATH_MAX];
//             snprintf(entry_path, PATH_MAX, "%s/%s", path, entry->d_name);

//             if (entry->d_type == DT_DIR) {
//                 remove_recursive(entry_path);  // Recursively remove subdirectories
//             } else {
//                 remove_file(entry_path);  // Remove files
//             }
//         }
//     }

//     closedir(dir);
//     remove_directory(path);  // Remove the current directory after its contents are removed
// }

// int my_rm(int argc, char *argv[]) {
//     printf("argc= %d\n",argc);
//     printf("*argv[0]= %s\n",&*argv[0]);
//     printf("*argv[1]= %s\n",&*argv[1]);
//     int force = 0;
//     int recursive = 0;

//     int opt;
//     while ((opt = getopt(argc, argv, "fir")) != -1) {
//         switch (opt) {
//             case 'f':
//                 force = 1;
//                 break;
//             case 'i':
//                 interactive = 1;
//                 printf("i\n");
//                 break;
//             case 'r':
//                 recursive = 1;
//                 break;
//             default:
//                 fprintf(stderr, "rm: Usage: %s [-fir] <file1> [file2] [file3] ...\n", argv[0]);
//                 exit(EXIT_FAILURE);
//         }
//     }

//     if (optind == argc) {
//         fprintf(stderr, "Usage: %s [-fir] <file1> [file2] [file3] ...\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }

//     for (int i = optind; i < argc; i++) {
//         char *path = argv[i];

//         if (recursive) {
//             remove_recursive(path);
//         } 
//         else if(interactive){
//         remove_file(path);
//         printf("Removed: %s\n", path); 
//         }
//         else 
//         {
//                 if (!force) {
//                     perror("Error removing");
//                     exit(EXIT_FAILURE);
//                 }
//         }
        
//     }
//     return 0;
//     //exit(EXIT_SUCCESS);
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

void clear_input_buffer_rm() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void remove_directory(char *dir_path, int force, int interactive) {
    if (!force && interactive) {
        char response[BUFFER_SIZE];
        printf("删除目录 %s 及其内容? (y/n): ", dir_path);
        fgets(response, sizeof(response), stdin);

        // 清空输入缓冲区
        //clear_input_buffer_rm();

        if (response[0] != 'y' && response[0] != 'Y') {
            return;
        }
    }

    if (rmdir(dir_path) == -1) {
        perror("删除目录出错");
    }
}

void remove_file(char *file_path, int force, int interactive) {
    if (!force && interactive) {
        char response[BUFFER_SIZE];
        printf("删除文件 %s? (y/n): ", file_path);
        fgets(response, sizeof(response), stdin);

        // 清空输入缓冲区
        //clear_input_buffer_rm();

        if (response[0] != 'y' && response[0] != 'Y') {
            return;
        }
    }

    if (unlink(file_path) == -1) {
        perror("删除文件出错");
    }
}

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

int my_rm(int argc, char *argv[]) {
    int force = 0;
    int recursive = 0;
    int interactive = 0;
    optind = 1;
    int opt;
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

    for (int i = optind; i < argc; i++) {
        char *path = argv[i];

        struct stat path_stat;
        if (stat(path, &path_stat) == -1) {
            perror("获取文件/目录信息出错");
        }

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

// #define _GNU_SOURCE
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <dirent.h>
// #include <string.h>
// #include <limits.h>
// #include <getopt.h>

// #define PATH_MAX 4096


// void remove_file(char *file_path,int interactive) {
//     if (interactive) {
//         char response;
//         printf("Remove file %s? (y/n): ", file_path);
//         scanf(" %c", &response);
//         if (response != 'y') {
//             printf("Skipped: %s\n", file_path);
//             return;
//         }
//     }

//     if (unlink(file_path) == -1) {
//         perror("Error removing file");
//         exit(EXIT_FAILURE);
//     }
// }

// void remove_directory(char *dir_path,int interactive) {
//     if (interactive) {
//         char response;
//         printf("Remove directory %s (y/n): ", dir_path);
//         scanf(" %c", &response);
//         if (response != 'y') {
//             printf("Skipped: %s\n", dir_path);
//             return;
//         }
//     }

//     if (rmdir(dir_path) == -1) {
//         perror("Error removing directory");
//         exit(EXIT_FAILURE);
//     }
// }

// void remove_recursive(char *path,int interactive) {
//     DIR *dir = opendir(path);
//     struct dirent *entry;

//     if (!dir) {
//         perror("Error opening directory");
//         exit(EXIT_FAILURE);
//     }

//     while ((entry = readdir(dir)) != NULL) {
//         if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//             char entry_path[PATH_MAX];
//             snprintf(entry_path, PATH_MAX, "%s/%s", path, entry->d_name);

//             if (entry->d_type == DT_DIR) {
//                 remove_recursive(entry_path,interactive);  // Recursively remove subdirectories
//             } else {
//                 remove_file(entry_path,interactive);  // Remove files
//             }
//         }
//     }

//     closedir(dir);
//     remove_directory(path,interactive);  // Remove the current directory after its contents are removed
// }

// int my_rm(int argc, char *argv[]) {
//     int force = 0;
//     int recursive = 0;
//     int interactive = 0;
//     int opt;
//     while ((opt = getopt(argc, argv, "fir")) != -1) {
//         switch (opt) {
//             case 'f':
//                 force = 1;
//                 break;
//             case 'i':
//                 interactive = 1;
//                 break;
//             case 'r':
//                 recursive = 1;
//                 break;
//             default:
//                 fprintf(stderr, "rm: Usage: %s [-fir] <file1> [file2] [file3] ...\n", argv[0]);
//                 exit(EXIT_FAILURE);
//         }
//     }

//     if (optind == argc) {
//         fprintf(stderr, "Usage: %s [-fir] <file1> [file2] [file3] ...\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }

//     for (int i = optind; i < argc; i++) {
//         char *path = argv[i];

//         if (recursive) {
//             remove_recursive(path,interactive);
//         } else {
//             ///////////////////////
//             if(interactive)
//             {
//             char response;
//             printf("Remove file %s? (y/n): ", path);
//             scanf(" %c", &response);
//               if (response != 'y') {
//                printf("Skipped: %s\n", path);
//               }
//               else{
//                 if (unlink(path) == 0) {
//                 printf("Removed: %s\n", path);
//                 } else {
//                 if (!force) {
//                     perror("Error removing");
//                     exit(EXIT_FAILURE);
//                 }
//             }
//               }
//             }
//             else{
//             ////////////
//             if (unlink(path) == 0) {
//                 printf("Removed: %s\n", path);
//             } else {
//                 if (!force) {
//                     perror("Error removing");
//                     exit(EXIT_FAILURE);
//                 }
//             }
//             //////////////
//             }
//         }
//     }
//     return 0;
//     //exit(EXIT_SUCCESS);
// }







