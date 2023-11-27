#ifndef CP_H
#define CP_H

int my_cp(int argc, char **argv);
int cp_file_to_file(const char *src_file, const char *dest_file);
int cp_file_to_dir(const char *src_file, const char *dest_dir);
int cp_dir_recursive(const char *src_dir, const char *dest_dir);
#endif 
