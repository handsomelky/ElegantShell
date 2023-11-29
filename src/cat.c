#include <stdio.h>
#include <string.h>
#include "cat.h"

int cat(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [file1 file2 ...] [-n filename]\n", argv[0]);
        return 1;
    }
    else{
        int flag = 0, num = 1;
        if(strcmp(argv[1], "-n") == 0)
            flag = 1;
        for(int i = 1 + flag; i < argc; i++){
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                fprintf(stderr,"%s:No such file or directory", argv[0]);
                return 1;
            }
            int ch;
            if(flag)
                printf("%d ",num++);
            while ((ch = fgetc(file)) != EOF){
                if(flag && ch == '\n'){
                    printf("\n%d ",num++);
                    continue;
                }
                putchar(ch);    
            }
            fclose(file);
        }
    }
    return 0;
}
