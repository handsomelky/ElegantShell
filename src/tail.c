#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tail.h"

// 函数实现：显示文件的最后几行
int tail_command(int argc, char **argv) {
if (argc < 4 || strcmp(argv[1], "-n") != 0) {
fprintf(stderr, "用法: tail -n <行数> <文件名>\n");
return 1;
}

int lines_to_show = atoi(argv[2]);
const char *file_name = argv[3];

FILE *file = fopen(file_name, "r");
if (!file) {
perror("tail");
return 1;
}

fseek(file, 0, SEEK_END);
long file_size = ftell(file);

int lines_count = 0;
char ch;
for (long i = file_size - 1; i >= 0; i--) {
fseek(file, i, SEEK_SET);
ch = fgetc(file);
if (ch == '\n') {
lines_count++;
if (lines_count == lines_to_show + 1) {
break;
}
}
}

fseek(file, ftell(file) - 1, SEEK_SET); // 为了排除文件末尾可能的换行符

// 从当前位置开始读取并输出
while ((ch = fgetc(file)) != EOF) {
putchar(ch);
}

fclose(file);

return 0;
}
