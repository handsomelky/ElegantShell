#ifndef HISTORY_H__
#define HISTORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义历史记录中最大命令数量
#define MAX_HISTORY_SIZE 50

// 结构体表示历史记录中的一个命令
struct HistoryNode {
char command[256];
struct HistoryNode* next;
};

// 初始化历史记录
void init_history();

// 将命令添加到历史记录
void add_to_history(const char* command);

// 显示命令历史记录
void display_history();

int history_size();

#endif
