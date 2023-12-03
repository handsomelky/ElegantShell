#include "history.h"

static struct HistoryNode* history_head = NULL;

// 初始化历史记录
void init_history() {
history_head = NULL;
}

// 将命令添加到历史记录
void add_to_history(const char* command) {
// 创建一个新的历史记录节点
struct HistoryNode* new_node = (struct HistoryNode*)malloc(sizeof(struct HistoryNode));

// 将命令复制到新节点
strncpy(new_node->command, command, sizeof(new_node->command) - 1);
new_node->command[sizeof(new_node->command) - 1] = '\0';

// 将新节点添加到历史记录列表的最前面
new_node->next = history_head;
history_head = new_node;

// 限制历史记录的大小
if (history_size() > MAX_HISTORY_SIZE) {
// 如果历史记录超过限制，则删除最后一个节点
struct HistoryNode* prev = NULL;
struct HistoryNode* current = history_head;
while (current->next != NULL) {
prev = current;
current = current->next;
}

free(current);
if (prev != NULL) {
prev->next = NULL;
} else {
history_head = NULL;
}
}
}

// 显示命令历史记录
void display_history() {
printf("命令历史记录(使用时间由近到远):\n");
struct HistoryNode* current = history_head;
int count = 1;
while (current != NULL) {
printf("%d. %s", count, current->command);
current = current->next;
count++;
}
}

// 获取历史记录的大小
int history_size() {
int size = 0;
struct HistoryNode* current = history_head;
while (current != NULL) {
size++;
current = current->next;
}
return size;
}
