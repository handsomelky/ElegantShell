// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <dirent.h>
// #include <libgen.h>
// #include <errno.h>
// #define PATH_MAX        4096
// void copy_file(const char *source, const char *destination);
// void copy_directory(const char *source, const char *destination);

// int my_cp(int argc, char *argv[]) {
//     if (argc != 3) {
//         fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
//         //exit(EXIT_FAILURE);
//     }

//     const char *source = argv[1];
//     const char *destination = argv[2];

//     struct stat source_stat;
//     if (stat(source, &source_stat) == -1) {
//         perror("Error getting source file/directory information");
//         //exit(EXIT_FAILURE);
//     }

//     // 检查目标是否为目录
//     struct stat dest_stat;
//     if (stat(destination, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
//         // 如果目标是目录，则构造目标文件路径
//         char dest_path[PATH_MAX];
//         snprintf(dest_path, sizeof(dest_path), "%s/%s", destination, basename((char *)source));

//         // 创建目标目录
//         if (mkdir(dest_path, 0777) == -1 && errno != EEXIST) {
//             perror("Error creating destination directory");
//             //exit(EXIT_FAILURE);
//         }

//         // 调整 destination 为目标文件的完整路径
//         destination = dest_path;
//     }

//     if (S_ISREG(source_stat.st_mode)) {
//         copy_file(source, destination);
//     } else if (S_ISDIR(source_stat.st_mode)) {
//         copy_directory(source, destination);
//     } else {
//         fprintf(stderr, "Unsupported file type\n");
//         //exit(EXIT_FAILURE);
//     }

//     printf("Copy successful.\n");
//     return 0;
// }

// void copy_file(const char *source, const char *destination) {
//     // 检查目标是否为目录
//     struct stat dest_stat;
//     if (stat(destination, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
//         // // 如果目标是目录，则构造目标文件路径
//         // char dest_path[PATH_MAX];
//         // snprintf(dest_path, sizeof(dest_path), "%s/%s", destination, basename((char *)source));

//         // // 创建目标目录
//         // if (mkdir(destination, 0777) == -1) {
//         //     perror("Error creating destination directory");
//         //     //exit(EXIT_FAILURE);
//         // }
//         FILE *source_file = fopen(source, "rb");
//         if (source_file == NULL) {
//             perror("Error opening source file");
//             //exit(EXIT_FAILURE);
//         }

//         FILE *destination_file = fopen(destination, "wb");
//         if (destination_file == NULL) {
//             perror("Error opening destination file");
//             fclose(source_file);
//             //exit(EXIT_FAILURE);
//         }

//         char buffer[1024];
//         size_t bytesRead;

//         while ((bytesRead = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
//             fwrite(buffer, 1, bytesRead, destination_file);
//         }

//         fclose(source_file);
//         fclose(destination_file);
//     } else {
//         // 如果目标不是目录，则继续原有的代码
//         FILE *source_file = fopen(source, "rb");
//         if (source_file == NULL) {
//             perror("Error opening source file");
//             //exit(EXIT_FAILURE);
//         }

//         FILE *destination_file = fopen(destination, "wb");
//         if (destination_file == NULL) {
//             perror("Error opening destination file");
//             fclose(source_file);
//             //exit(EXIT_FAILURE);
//         }

//         char buffer[1024];
//         size_t bytesRead;

//         while ((bytesRead = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
//             fwrite(buffer, 1, bytesRead, destination_file);
//         }

//         fclose(source_file);
//         fclose(destination_file);
//     }
// }


// void copy_directory(const char *source, const char *destination) {
//     DIR *dir = opendir(source);
//     if (dir == NULL) {
//         perror("Error opening source directory");
//         //exit(EXIT_FAILURE);
//     }

//     if (mkdir(destination, 0777) == -1) {
//         perror("Error creating destination directory");
//         closedir(dir);
//         //exit(EXIT_FAILURE);
//     }

//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//             char source_path[PATH_MAX];
//             char destination_path[PATH_MAX];

//             snprintf(source_path, sizeof(source_path), "%s/%s", source, entry->d_name);
//             snprintf(destination_path, sizeof(destination_path), "%s/%s", destination, entry->d_name);

//             struct stat entry_stat;
//             if (stat(source_path, &entry_stat) == -1) {
//                 perror("Error getting file/directory information");
//                 closedir(dir);
//                 //exit(EXIT_FAILURE);
//             }

//             if (S_ISREG(entry_stat.st_mode)) {
//                 copy_file(source_path, destination_path);
//             } else if (S_ISDIR(entry_stat.st_mode)) {
//                 copy_directory(source_path, destination_path);
//             }
//         }
//     }

//     closedir(dir);
// }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
// Function declarations
void copy_file(const char *src_path, const char *dest_path, int force, int interactive, int verbose, int backup);
void copy_directory(const char *src_path, const char *dest_path, int force,int recursive, int interactive, int verbose, int backup);
//void print_help();

int my_cp(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [-options] source destination\n", argv[0]);
        //print_help();
        exit(EXIT_FAILURE);
    }

    // Parse command line options
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
                print_help();
                exit(EXIT_FAILURE);
        }
    }

    // Determine source and destination paths
    const char *src_path = argv[optind];
    const char *dest_path = argv[optind + 1];

    if (link_) {
        // Create a hard link
        if (link(src_path, dest_path) != 0) {
            perror("Error creating hard link");
            exit(EXIT_FAILURE);
        }
    } else {
        struct stat src_stat;
        if (lstat(src_path, &src_stat) == -1) {
            perror("Error getting file information");
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(src_stat.st_mode)) {
            // Copy directory
            copy_directory(src_path, dest_path, force, recursive,interactive, verbose, backup);
        } else if (S_ISREG(src_stat.st_mode) || (S_ISLNK(src_stat.st_mode) && copy_symlink)) {
            // Copy regular file or symbolic link
            copy_file(src_path, dest_path, force, interactive, verbose, backup);
        } else {
            fprintf(stderr, "Unsupported file type\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("Copy successful\n");

    return 0;
}

void copy_file(const char *src_path, const char *dest_path, int force, int interactive, int verbose, int backup) {
    struct stat src_stat, dest_stat;

    if (lstat(dest_path, &dest_stat) == 0) {
        //Destination file exists
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
            // Prompt before overwrite
            char response;
            printf("Do you want to overwrite '%s'? (y/n): ", dest_path);
            scanf(" %c", &response);

            if (response != 'y') {
                printf("File not copied.\n");
                return;
            }
        } else if (!force) {
            // Without -f option, do not overwrite without confirmation
            if(!backup)
            {
            printf("File '%s' already exists. Use -f to force copy.\n", dest_path);
            return;
            }
        }

        if (backup) {
            // Backup the existing file
            char backup_path[BUFSIZE];
            snprintf(backup_path, BUFSIZE, "%s.bak", dest_path);

            if (rename(dest_path, backup_path) == -1) {
                perror("Error creating backup");
                exit(EXIT_FAILURE);
            }
            if (verbose) {
                printf("Backup created: %s\n", backup_path);
            }
           }
        } else {
            fprintf(stderr, "Unsupported file type\n");
            exit(EXIT_FAILURE);
        }
        
    }

    FILE *src_fp = fopen(src_path, "rb");
    if (src_fp == NULL) {
    perror("Error opening source file");
    exit(EXIT_FAILURE);
    }

    FILE *dest_fp = fopen(dest_path, "wb");
    if (dest_fp == NULL) {
    perror("Error opening destination file");
    fclose(src_fp);
    exit(EXIT_FAILURE);
    }

    
    ssize_t bytesRead, bytesWritten;
    char buffer[BUFSIZE];
    
    
    while ((bytesRead = fread(buffer, 1,BUFSIZE,src_fp)) > 0) {

        bytesWritten = fwrite(buffer, 1, bytesRead, dest_fp);
        if (bytesWritten != bytesRead) {
            fprintf(stderr,"Error writing to destination file");
             fclose(src_fp);
             fclose(dest_fp);
            exit(EXIT_FAILURE);
        }
    }

    fclose(src_fp);
    fclose(dest_fp);

    if (verbose) {
        printf("File copied: %s -> %s\n", src_path, dest_path);
    }
}

void copy_directory(const char *src_path, const char *dest_path, int force,int recursive, int interactive, int verbose, int backup) {
    if(recursive)
    {
    struct stat src_stat;

    if (lstat(src_path, &src_stat) == -1) {
        perror("Error getting source directory information");
        exit(EXIT_FAILURE);
    }

    if (!S_ISDIR(src_stat.st_mode)) {
        fprintf(stderr, "Error: Source '%s' is not a directory.\n", src_path);
        exit(EXIT_FAILURE);
    }

    DIR *src_dir = opendir(src_path);
    if (!src_dir) {
        perror("Error opening source directory");
        exit(EXIT_FAILURE);
    }

    char dest_dir_path[BUFSIZE];
    snprintf(dest_dir_path, BUFSIZE, "%s/%s", dest_path, strrchr(src_path, '/') + 1);
    mkdir(dest_dir_path, 0777);

    struct dirent *entry;
    while ((entry = readdir(src_dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Skip "." and ".."
        }

        char src_child_path[BUFSIZE];
        char dest_child_path[BUFSIZE];
        /////////////////////////////////////////////////////////////
        char absolute_src_path[PATH_MAX];
        char absolute_dest_path[PATH_MAX];
        realpath(src_child_path, absolute_src_path);
        realpath(dest_child_path, absolute_dest_path);
        printf("Source Path: %s\n", absolute_src_path);
        printf("Destination Path: %s\n", absolute_dest_path);
////////////////////////////////////////////////////////////////////////
        snprintf(src_child_path, BUFSIZE, "%s/%s", src_path, entry->d_name);
        snprintf(dest_child_path, BUFSIZE+300, "%s/%s", dest_dir_path, entry->d_name);//dest_path

        struct stat child_stat;
        if (lstat(src_child_path, &child_stat) == -1) {
            perror("Error getting child file information");
            closedir(src_dir);
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(child_stat.st_mode)) {
            // Recursively copy subdirectories
            copy_directory(src_child_path, dest_child_path, force,recursive,interactive, verbose, backup);
        } else if (S_ISREG(child_stat.st_mode)) {
            // Copy regular files
            copy_file(src_child_path, dest_child_path, force, interactive, verbose, backup);
        } else if (S_ISLNK(child_stat.st_mode)) {
            // Copy symbolic links
            char link_target[BUFSIZE];
            ssize_t len = readlink(src_child_path, link_target, sizeof(link_target) - 1);
            if (len == -1) {
                perror("Error reading link target");
                closedir(src_dir);
                exit(EXIT_FAILURE);
            }
            link_target[len] = '\0';

            if (backup) {
                // Backup the existing symbolic link
                char backup_path[BUFSIZE];
                snprintf(backup_path, BUFSIZE+4, "%s.bak", dest_child_path);

                if (rename(dest_child_path, backup_path) == -1) {
                    perror("Error creating backup");
                    closedir(src_dir);
                    exit(EXIT_FAILURE);
                }
                if (verbose) {
                    printf("Backup created: %s\n", backup_path);
                }
            }

            if (interactive) {
                // Prompt before overwrite for symbolic links
                char response;
                printf("Do you want to overwrite '%s'? (y/n): ", dest_child_path);
                scanf(" %c", &response);

                if (response != 'y') {
                    printf("Symbolic link not copied.\n");
                    continue;
                }
            } else if (!force) {
                // Without -f option, do not overwrite without confirmation
                printf("Symbolic link '%s' already exists. Use -f to force copy.\n", dest_child_path);
                continue;
            }

            if (symlink(link_target, dest_child_path) == -1) {
                perror("Error creating symbolic link");
                closedir(src_dir);
                exit(EXIT_FAILURE);
            }
            if (verbose) {
                printf("Symbolic link copied: %s -> %s\n", src_child_path, dest_child_path);
            }
        } else {
            fprintf(stderr, "Unsupported file type\n");
            closedir(src_dir);
            exit(EXIT_FAILURE);
        }
    }

    closedir(src_dir);

    if (verbose) {
        printf("Directory copied: %s -> %s\n", src_path, dest_path);
    }
    }
    else
    {
      fprintf(stderr, "cp: -r not specified;\n");
      exit(EXIT_FAILURE);
    }
}




void print_help() {
    printf("Options:\n");
    printf("  -d: Create a hard link to the source file\n");
    printf("  -r, --recursive: Copy directories recursively\n");
    printf("  -i, --interactive: Prompt before overwrite\n");
    printf("  -f, --force: Force copy, do not prompt for confirmation\n");
    printf("  -v, --verbose: Display detailed copy process\n");
    printf("  -l: Create a hard link to the source file (do not copy)\n");
    printf("  -b: Backup overwritten files or directories\n");
}



