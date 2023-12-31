#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <getopt.h>

#include <ctype.h>
#include <stdbool.h>
#define PROGRAM_NAME "gut"
#define FILE_NAME "numbers.txt"

#define PROGRAM_NAME "gut"
#define FILE_NAME "numbers.txt"


void copy_stream(FILE *src, FILE *dst){
    char c;
    while ((c = getc(src)) != EOF){
        putc(c,dst);
    }
    return;
}

void process_stream(FILE *stream, int li, int tl, int hl){// line_index, tail_lines, header_lines
    int head_line = li - hl;
    int tail_line = li + tl;
    char c;
    int lines = 1;
    while ((c = getc(stream)) != EOF){
        if ((lines >= head_line && lines <= tail_line)){ //Removed  "&& c != '\n'". May cause problems with extra lines.
            putc(c,stdout);
        }
        if (c == '\n'){
            lines++;
        }
        if (lines > tail_line){
            break;
        }
    }
    return;
}

void process_chunk(FILE *stream, int li, int tl, int hl) {
    char c, last_c;
    int header_line = li-hl;
    int lines = 1;
    while ((c = getc(stream)) != EOF){
        if (lines >= header_line){
            putc(c,stdout);
        }
        if (c == '\n'){
            lines++;
        }
        if(c == '\n' && last_c == '\n' && lines > li){
            break;
        }
        last_c = c;
    }
    return;
}

void print_help(){
    puts(
        "Usage: gut [OPTION]... [FILE]...\n"
        "Print all lines of FILE or chunks of FILE.\n"
        "\n"
        "With no FILE, or when FILE is -, read standard input.\n"
        "With no OPTION gut outputs the first five lines to stdout.\n"
        "-n NUM		set the line index to NUM\n"
        "-u NUM		read NUM lines above the index\n"
        "-d NUM         read NUM lines below the index\n"
        "-c         chunk mode. Read until the next empty line.\n"
        "\n"
        "https://github.com/Kango-Senpai/gut"
    );

    return;
}


int main(int argc, char **argv){
    if (argc <= 1){
        printf("No arguments provided, reading from stdin.\n");
        copy_stream(stdin,stdout);
        return 0;
    }
    void (*process_function_ptr)(FILE*,int,int,int) = &process_stream; //Set the default processing method. This will change if '-c' is passed.
    //Program options. Will later be moved to a struct. stuct line_options???
    int line_index = 0, head_lines = 5, tail_lines = 5;
    char *c = malloc(sizeof(char));

    opterr = 0;
    //Get command line flags and set program options.
    while ((*c = getopt(argc,argv,"cd:hn:u:")) != EOF){
        switch (*c){
            case 'd':
                tail_lines = atoi(optarg);
                break;
            case 'n':
                line_index = atoi(optarg);
                //If the line index is 0, a non-numeric argument was provided.
                break;
            case 'u':
                head_lines = atoi(optarg);
                break;
            case 'c':
                process_function_ptr = &process_chunk;
                break;
            case 'h':
                print_help();
                free(c);
                return 0;
            case '?':
                if (optopt == 'n'){
                    fprintf(stderr,"Option '-%c' requires an argument.\n",optopt);
                }
                else if (isprint(optopt)){
                    fprintf(stderr,"Unknown option '-%c'.\n",optopt);
                    print_help();

                }
                else {
                    fprintf(stderr,"Arguments contained an invalid character. ('\\x%x')\n",optopt);
                }
                return 1;
        }
    }
    free(c);
    //Non-option arguments shall be treated as files.
    FILE *stream = NULL;
    //Try to parse remaining arguments as file names. If the stream is never set, it is assigned stdin.
    //TODO:
    for (; optind < argc; optind++){
        stream = fopen(argv[optind],"r");
        if (stream == NULL){
            printf("Unable to read file: %s\n",argv[optind]);
            continue;
        }
        (*process_function_ptr)(stream,line_index,tail_lines,head_lines);
    }
    if (stream == NULL){
        stream = stdin;
        (*process_function_ptr)(stream,line_index,tail_lines,head_lines);

    }
    
    // If argc is 2, user provided either a flag or a file.
    // Figure out lag parsing...
    
    //printf("Reading line %d from %s.\n",line_to_read,FILE_NAME);
    
    //Following code counts the number of '\n' characters in a file to determine the number of lines.
    //Sequential read sounds cool ;)
    
    fclose(stream);
    return 0;
}