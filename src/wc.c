#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#include "wc.h"

#define RESET "\x1B[0m"
#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE    "\x1B[34m"

int wc(int argc, char *argv[]) {
    bool count_lines = false, count_words = false, count_chars = false;
    bool options_present = false;
    bool input_from_redirection = false;
    int start_index = 1; // 用于跟踪从哪里开始处理非选项参数

    // 解析选项
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'i': 
                        input_from_redirection = true;
                        break;
                    case 'l': 
                        count_lines = true; 
                        options_present = true;
                        break;
                    case 'w': 
                        count_words = true; 
                        options_present = true;
                        break;
                    case 'c': 
                        count_chars = true; 
                        options_present = true;
                        break;
                    default:
                        fprintf(stderr, "wc: invalid option -- '%c'\n", argv[i][j]);
                        return -1;
                }
            }
            start_index++;
        }
    }

    // 如果没有指定任何选项，默认全部统计
    if (!options_present) {
        count_lines = count_words = count_chars = true;
    }

    // 从输入重定向处理
    if (input_from_redirection) {
        long l_count = 0, w_count = 0, c_count = 0;

        for (int i = start_index; i < argc; i++) { // 跳过命令名和所有选项
            char *input = argv[i];
            l_count++; // 每个参数代表一行
            w_count++; // 每个参数代表一个单词

            for (int j = 0; input[j] != '\0'; j++) {
                if (count_chars) {
                    c_count++; // 统计字符
                }
            }
            if (count_chars) {
                c_count++; // 为每行末尾的换行符计数
            }
        }
        l_count-=1;
        c_count-=1;
        // 输出统计结果
        if (count_lines) printf(RED "%ld " RESET, l_count);
        if (count_words) printf(GREEN "%ld " RESET, w_count);
        if (count_chars) printf(BLUE "%ld " RESET, c_count);
        printf("\n");
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] != '-') {
                FILE *fp = (strcmp(argv[i], "-") == 0) ? stdin : fopen(argv[i], "r");
                if (fp == NULL) {
                    perror(argv[i]);
                    continue;
                }

                long l_count = 0, w_count = 0, c_count = 0;
                int c;
                bool in_word = false;

                while ((c = fgetc(fp)) != EOF) {
                    if (count_chars) {
                        c_count++;
                    }
                    if (c == '\n' && count_lines) {
                        l_count++;
                    }
                    if (isspace(c)) {
                        if (in_word) {
                            in_word = false;
                            if (count_words) {
                                w_count++;
                            }
                        }
                    } else {
                        in_word = true;
                    }
                }
                if (in_word && count_words) {
                    w_count++;
                }

                if (count_lines) printf(RED "%ld " RESET, l_count);
                if (count_words) printf(GREEN "%ld " RESET, w_count);
                if (count_chars) printf(BLUE "%ld " RESET, c_count);
                printf(YELLOW "%s\n" RESET, (fp == stdin) ? "stdin" : argv[i]);

                if (fp != stdin) {
                    fclose(fp);
                }
            }
        }

    }
    return 0;
}