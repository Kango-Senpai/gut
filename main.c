#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PROGRAM_NAME "gut"
#define FILE_NAME "numbers.txt"

int copy_stream(FILE *src, FILE *dst){
    char c;
    while ((c = getc(src)) != EOF){
        putc(c,dst);
    }
}

int main(int argc, char **argv){
    if (argc <= 1){
        //printf("Usage: gut [OPTION] [FILE]...\n");
        copy_stream(stdin,stdout);
        return 0;
    }
    // If argc is 2, user provided either a flag or a file.
    // Figure out flag parsing...
    FILE *fp = fopen(FILE_NAME,"r");
    if (fp == NULL){
        printf("Couldn't read file: %s.\n",FILE_NAME);
        return 1;
    }
    // switch (argc){
    //     case 2:
    //         file_to_stdout(fp);
    // }
    long line_to_read = 1;
    line_to_read -= 1 ? line_to_read > 0 : 0;
    //printf("Reading line %d from %s.\n",line_to_read,FILE_NAME);
    
    //Following code counts the number of '\n' characters in a file to determine the number of lines.
    //Sequential read sounds cool ;)
    int lines = 0;
    char c;
    char *buf = malloc(sizeof(char)*20);
    while ((c = getc(fp)) != EOF){
        if (lines == line_to_read && c != '\n'){
            strcat(buf,&c);
        }
        if (c == '\n'){
            lines++;
        }
        if (lines > line_to_read){
            break;
        }
    }
    
    if (line_to_read > lines){
        printf("Line index is greater than file length.\n");
    }
    else{
        printf("Line %i: %s",line_to_read,buf);
    }
    free(buf);
    fclose(fp);
    return 0;
}