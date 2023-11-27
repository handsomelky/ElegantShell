#ifndef RM_H
#define RM_H

int my_rm(int argc, char **argv) ;
//int rm_recursive(const char *path);
void remove_file(char *file_path);

void remove_directory(char *dir_path);

void remove_recursive(char *path);
#endif
