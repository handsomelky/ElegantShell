#include "ps.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

// 修改 print_process_info 函数，使其符合 bst_insert 函数期望的处理函数
int print_process_info(int argc, char **argv) {
DIR *dir;
struct dirent *entry;

// 打开 proc 目录
if ((dir = opendir("/proc")) == NULL) {
perror("opendir");
return 1; // 返回错误码表示处理失败
}

printf("%-10s %-10s %-10s %-10s %-10s\n", "PID", "STATUS", "UTIME", "STIME", "VSIZE");

// 遍历 proc 目录
while ((entry = readdir(dir)) != NULL) {
if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
char path[512];
// 增加缓冲区大小
snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);

FILE *stat_file = fopen(path, "r");
if (stat_file == NULL) {
perror("fopen");
continue;
}

struct ProcessInfo process_info;
fscanf(stat_file, "%d %*s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %lu %lu",
&process_info.pid, &process_info.status, &process_info.utime, &process_info.stime);

fclose(stat_file);

// 获取虚拟内存大小
// 增加缓冲区大小
snprintf(path, sizeof(path), "/proc/%s/statm", entry->d_name);
FILE *statm_file = fopen(path, "r");
if (statm_file == NULL) {
perror("fopen");
continue;
}
fscanf(statm_file, "%lu", &process_info.vsize);

fclose(statm_file);

// 打印进程信息
printf("%-10d %-10c %-10lu %-10lu %-10lu\n",
process_info.pid, process_info.status, process_info.utime, process_info.stime, process_info.vsize);
}
}

closedir(dir);

return 0; // 返回成功码表示处理成功
}

