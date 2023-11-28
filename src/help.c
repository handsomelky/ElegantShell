#include "help.h"
#include <stdio.h>

// 修改 display_help 函数的参数
int display_help(int argc, char **argv) {
printf("ElegantShell Help:\n");
printf(" hello - Print a greeting message\n");
printf(" tree - Display directory tree structure\n");
printf(" wc - Count lines, words, and characters in a file\n");
printf(" ps - Display process information\n");
printf(" help - Display help information\n");
printf(" exit - Exit the shell\n");
// Add more commands as needed

printf("\nUsage:\n");
printf(" Command [options] [arguments]\n");
printf(" For example: hello -n John\n");
printf("\nNote: Some commands may support additional options and arguments.\n");

// 修改 display_help 函数的返回值，以符合期望的函数签名
return 0;
}
