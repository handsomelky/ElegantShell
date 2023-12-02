#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "head.h"

// 函数实现：显示文件的前面几行
int head_command(int argc, char **argv) {
if (argc < 4 || strcmp(argv[1], "-n") != 0) {
fprintf(stderr, "用法: head -n <行数> <文件名>\n");
return 1;
}

int lines_to_show = atoi(argv[2]);
const char *file_name = argv[3];

FILE *file = fopen(file_name, "r");
if (!file) {
perror("head");
return 1;
}

int lines_count = 0;
char ch;
while ((ch = fgetc(file)) != EOF && lines_count < lines_to_show) {
putchar(ch);
if (ch == '\n') {
lines_count++;
}
}

fclose(file);

return 0;
}
