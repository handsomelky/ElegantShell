#ifndef PS_H__
#define PS_H__

// 结构体表示进程信息
struct ProcessInfo {
int pid;
char status;
unsigned long utime;
unsigned long stime;
unsigned long vsize;
};

// 获取进程信息并打印
int print_process_info();

#endif
