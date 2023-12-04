#include <stdio.h>
#include <string.h>
#include "cat.h"

int cat(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s [file1 file2 ...] [-n filename]\n", argv[0]);
        return 1;
    } 
    else {
        int flag = 0, num = 1;   // flag用于标记是否使用了-n选项，num用于记录行号。
        // 如果第一个参数是-n，则启用行号标记
        if (argv[1][0] == '-') {
            if (strcmp(argv[1], "-n") == 0)
                flag = 1;
            else {
                fprintf(stderr, "Usage: %s [file1 file2 ...] [-n filename]\n", argv[0]);
                return 1;
            }
        }
        // 如果只有一个参数，即未指定文件，则返回错误
        if (argc < 2 + flag) {
            fprintf(stderr, "Usage: %s [file1 file2 ...] [-n filename]\n", argv[0]);
            return 1;
        }
        // 如果指定了输出到文件的选项，则打开输出文件
        if (argc >= 4 + flag && strcmp(argv[argc - 2], ">>") == 0) {
            FILE *outputFile = fopen(argv[argc - 1], "a");
            if (outputFile == NULL) {
                fprintf(stderr, "Error opening output file");
                return 1;
            }
            for (int i = 1 + flag; i < argc - 2; i++) {
                FILE *inputFile = fopen(argv[i], "r");
                if (inputFile == NULL) {
                    fprintf(stderr, "No such file: %s\n", argv[i]);
                    fclose(outputFile);
                    return 1;
                }
                int ch;
                if (flag)
                    fprintf(outputFile, "%d ", num++);

                while ((ch = fgetc(inputFile)) != EOF) {
                    if (flag && ch == '\n') {
                        fprintf(outputFile, "\n%d ", num++);
                        continue;
                    }
                    fputc(ch, outputFile);
                }
                fclose(inputFile);
            }
            fclose(outputFile);
        } 
        else {
            // 没有输出到文件的选项，直接输出到终端
            for (int i = 1 + flag; i < argc; i++) {
                FILE *file = fopen(argv[i], "r");
                if (file == NULL) {
                    fprintf(stderr, "No such file: %s\n", argv[i]);
                    return 1;
                }
                int ch;
                if (flag)
                    printf("%d ", num++);
                while ((ch = fgetc(file)) != EOF) {
                    if (flag && ch == '\n') {
                        printf("\n%d ", num++);
                        continue;
                    }
                    putchar(ch);
                }
                if (flag)
                    printf("\n");
                fclose(file);
            }
        }
    }
    return 0;
}
