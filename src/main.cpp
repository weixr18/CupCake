#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cupcake.h"
#include "parser.h"

int main(int argc, char** argv){

    // read file
    char tmp[] = "D:/Codes/CXX/Cupcake/test/0.c";
    char* file_path = tmp;
    if(argc > 1){
        file_path = argv[1];
    }
    int code_length = 0;
    char* code = read_file(file_path, &code_length);

    // scan
    int token_count = 0;
    CUP_TOKEN* tokens;
    try{
        tokens = scan(code, code_length, &token_count);
    }
    catch (const char* s){
        printf("%s\n", s);
        exit(1);
    }
    for(int i = 0; i < token_count; i++){
        printf(token_to_grammar_tag(tokens[i]));
    }
    printf("\nToken number: %d\n", token_count);

    // parse
    CUP_ST* st;
    try{
        st = parse(tokens, token_count);
        print_st(st);
    }
    catch (char* s){
        printf("%s\n", s);
        exit(1);
    }
    catch (char const* s){
        printf("%s\n", s);
        exit(1);
    }

    return 0;
}