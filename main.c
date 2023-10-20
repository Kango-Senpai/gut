#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <getopt.h>

#include <ctype.h>
#define PROGRAM_NAME "gut"
#define FILE_NAME "numbers.txt"


int s_is_numeric(char *s){
    int len = sizeof(*s)/sizeof(s[0]);
    for (int i = 0; i < len; i++){
        if (!isdigit(s[i])){
            return 0;
        }
    }
    return 1;
}

int copy_stream(FILE *src, FILE *dst){
    char c;
    while ((c = getc(src)) != EOF){
        putc(c,dst);
    }
}


int main(int argc, char **argv){
    //Program options. Will later be moved to a struct.
    int line_index = -1;
    if (argc <= 1){
        printf("No arguments provided. Reading and writing to stdin and stdout.\n");
        copy_stream(stdin,stdout);
        return 0;
    }
    //Get command line flags and set program options.
    
    char c;

    opterr = 0;

    while ((c = getopt(argc,argv,"n:")) != EOF){
        switch (c){
            case 'n':
                line_index = atoi(optarg);
                //If the line index is 0, a non-numeric argument was provided.
                break;
            case '?':
                if (optopt == 'n'){
                    fprintf(stderr,"Option '-%c' requires an argument.\n",optopt);
                }
                else if (isprint(optopt)){
                    fprintf(stderr,"Unknown option '-%c'.\n",optopt);
                }
                else {
                    fprintf(stderr,"Arguments contained an invalid character. ('\\x%x')\n",optopt);
                }
                return 1;
        }
    }
    //Non-option arguments shall be treated as files.
    // for (; optind < argc; optind++){
        
    // }
    printf("%i\n",line_index);
    return 0;
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
    //char c;
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