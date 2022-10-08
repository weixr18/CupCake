/* 词法分析
* 输入代码字符串
* 输出token序列
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cupcake.h"

/*
windows换行为\r\n
*/

const int N_pun_symbols = 12;
char CUP_PUN_SYMBOLS[N_pun_symbols] = {
    '(', ')', '[', ']', '{', '}',
    ';', ',', '\'', '\"', ':', '?', 
};

const int N_opr_symbols = 13;
char CUP_OPR_SYMBOLS[N_opr_symbols] = {
    '+', '-', '*', '%', '/',
    '!', '~', '|', '&', '^',
    '<', '>', '=',
};


const int N_punctuators = 10;
char CUP_PUNCTUATORS[N_punctuators] = {
    '(', ')', '[', ']', '{', '}',
    ';', ',', ':', '?', 
};

const int N_t_operators = 27;
const char* CUP_T_OPERATORS[N_t_operators] = {
    "+", "-", "*", "/",  "%", 
    "<", ">", "<=", ">=", "==", "!="
    "|", "&", "^", "<<", ">>",
    "&&", "||", 
    "+=", "-=", "*=", "/=", 
    "|=", "&=", "^=", "<<=", ">>=",
    ", "
};

const int N_l_operators = 4;
const char* CUP_L_OPERATORS[N_l_operators] = {
    "++", "--", "!", "~",
    //"&",
};

const int N_r_operators = 2;
const char* CUP_R_OPERATORS[N_r_operators] = {
    "++", "--",
};

const int N_keywords = 12;
const char* CUP_KEYWORDS[N_keywords] = {
    "if", "do", "for", "else",
    "case", "break", "while", 
    "switch", "return", "sizeof",
    "default", "continue"
};

const int N_types = 8;
const char* CUP_TYPES[N_types] = {
    "int", "void", "char", "long",
    "bool", "short", "float", "double"
};

int num_token = 0;

bool is_blank_char(const char c){
    return c == ' ' || c == '\t' || c == '\n'|| c == '\r';
}

bool is_char(const char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool is_digit(const char c){
    return (c >= '0' && c <= '9') || (c == '.');
}

bool is_idf_chr(const char c){
    return is_char(c) || is_digit(c) || (c == '_');
}

bool is_pun_symbol(const char c){
    for(int i = 0; i < sizeof(CUP_PUN_SYMBOLS); i++){
        if(c == CUP_PUN_SYMBOLS[i]){
            return true;
        }
    }
    return false;
}

bool is_opr_symbol(const char c){
    for(int i = 0; i < sizeof(CUP_OPR_SYMBOLS); i++){
        if(c == CUP_OPR_SYMBOLS[i]){
            return true;
        }
    }
    return false;
}

CUP_E_CHAR_TYPE get_char_type(const char c){
    if(is_blank_char(c)){
        return CH_BLANK;
    }
    else if(is_char(c)){
        return CH_CHAR;
    }
    else if(is_digit(c)){
        return CH_DIGIT;
    }
    else if(is_opr_symbol(c)){
        return CH_OPR_SYMBOL;
    }
    else if(is_pun_symbol(c)){
        return CH_PUN_SYMBOL;
    }
    return CH_OTHER;
}

bool is_idf_chr(CUP_E_CHAR_TYPE t){
    return (t == CH_CHAR) || (t == CH_DIGIT);
}



CUP_TOKEN gen_pun_token(const char* s, int line, int col){
    CUP_TOKEN t;
    t.tid = num_token;
    t.t_class = CLS_PUNCTUATION;
    t.t_pos.line = line;
    t.t_pos.column = col;
    t.t_value.p = "\0";
    for(int i = 0; i < N_punctuators;i++){
        if(s[0] == CUP_PUNCTUATORS[i]){
            t.t_value.p = s;
        }
    }
    if(t.t_value.p == "\0"){
        throw "Error: Unknown punctuation.";
    }
  
    log_token(t); 
    return t;
}

CUP_TOKEN gen_opr_token(const char s[], int length, int line, int col, char latter){

    CUP_TOKEN t;
    t.tid = num_token;
    t.t_class = CLS_OPERATOR;
    t.t_pos.line = line;
    t.t_pos.column = col;

    bool hit = false;

    // =
    if(length == 1){
        if(is_str_equal(s, "=", 1)){
            t.t_value.o = s;
            t.t_sub.o = OPR_TYPE_EQU;
            hit = true;
        }
        else if(is_str_equal(s, "-", 1)){
            t.t_value.o = s;
            t.t_sub.o = OPR_TYPE_MINUS;
            hit = true;
        }
    }

    // two operator
    if(!hit){
        for(int i = 0; i < N_t_operators; i++){
            if(is_str_equal(s, CUP_T_OPERATORS[i], length)){
                t.t_value.o = s;
                t.t_sub.o = OPR_TYPE_TWO;
                hit = true;
                break;
            }
        }
    }
    
    // left/right operator
    if(!hit){
        if(is_str_equal(s, "++", length) || is_str_equal(s, "--", length)){
            if(is_char(latter)){
                t.t_value.o = s;
                t.t_sub.o = OPR_TYPE_LEFT;
                hit = true;
            }
            else{
                t.t_value.o = s;
                t.t_sub.o = OPR_TYPE_RIGHT;
                hit = true;
            }
        }
        else if(is_str_equal(s, "!", length) || is_str_equal(s, "~", length)){
            t.t_value.o = s;
            t.t_sub.o = OPR_TYPE_LEFT;
            hit = true;
        }
    }
    
    if (!hit){
        throw "Error: Unrecognized Operation.";
    }
    log_token(t); 
    return t;
}

CUP_TOKEN gen_digit_token(const char s[], int length, int line, int col){

    CUP_TOKEN t;
    t.tid = num_token;
    t.t_class = CLS_NUMBER;
    t.t_pos.line = line;
    t.t_pos.column = col;

    int count = count_char_in_str('.', s, length);
    if(count == 0){
        t.t_value.n.i = atoi(s);
        t.t_sub.n = NUM_TYPE_INT;
    }
    else if (count == 1){
        t.t_value.n.d = atof(s);
        t.t_sub.n = NUM_TYPE_DOUBLE;
    }
    else{
        throw "Error: Cannot identify number.";
    }

    log_token(t); 
    return t;
}

CUP_TOKEN gen_digit_token(const char c, int line, int col){
    CUP_TOKEN t;
    t.tid = num_token;
    t.t_class = CLS_NUMBER;
    t.t_pos.line = line;
    t.t_pos.column = col;
    t.t_value.n.c = c;
    t.t_sub.n = NUM_TYPE_CHAR;
    log_token(t); 
    return t;
}

CUP_TOKEN gen_idf_or_key_or_type_token(const char s[], int length, int line, int col){
    
    CUP_TOKEN t;
    t.tid = num_token;
    t.t_pos.line = line;
    t.t_pos.column = col;
    t.t_class = CLS_IDENTIFIER;
    t.t_value.i = s;

    for(int i = 0; i < N_keywords; i++){
        if(length != strlen(CUP_KEYWORDS[i])){
            continue;
        } 
        if(is_str_equal(s, CUP_KEYWORDS[i], length)){
            t.t_class = CLS_KEYWORD;
            t.t_value.k = s;
            break;
        }
    }
    for(int i = 0; i < N_types; i++){
        if(length != strlen(CUP_TYPES[i])){
            continue;
        } 
        if(is_str_equal(s, CUP_TYPES[i], length)){
            t.t_class = CLS_TYPE;
            t.t_value.t = s;
            break;
        }
    }

    log_token(t);    
    return t;
}

CUP_TOKEN gen_str_token(const char s[], int length, int line, int col){

    CUP_TOKEN t;
    t.tid = num_token;
    t.t_pos.line = line;
    t.t_pos.column = col;
    t.t_class = CLS_STRING;
    t.t_value.s = s;
    log_token(t);
    return t;
}

CUP_TOKEN* scan(const char code[], CUP_UI code_length, int* token_count){

    int tmp = code_length * sizeof(CUP_E_CHAR_TYPE);
    CUP_E_CHAR_TYPE* ch_types = (CUP_E_CHAR_TYPE *) malloc(tmp);
    for(int i = 0; i < code_length; i++){
        ch_types[i] = get_char_type(code[i]);
    }

    CUP_TOKEN* tokens = new CUP_TOKEN[code_length/2];
    int idx = 0;
    int line_index = 1;
    int col_index = 1;
    while(idx < code_length){

        // if(code[idx] == '*'){
        //     printf("2333333");
        // }
        int token_len = 0;
        
        // digit
        if(ch_types[idx] == CH_DIGIT){
            int i = 0;
            for(; i<(code_length-idx);i++){
                if(ch_types[idx+i] != CH_DIGIT){
                    break;
                }
            }
            char* piece = substr(code, code_length, idx, idx+i);
            tokens[num_token] = gen_digit_token(piece, i, line_index, col_index);
            // delete piece;
            num_token++;
            token_len = i;
        }

        // chars
        else if(ch_types[idx] == CH_CHAR){
            int i = 0;
            for(; i<(code_length-idx);i++){
                if(!is_idf_chr(ch_types[idx+i])){
                    break;
                }
            }
            char* piece = substr(code, code_length, idx, idx+i);
            tokens[num_token] = gen_idf_or_key_or_type_token(piece, i, line_index, col_index);
            // delete piece;
            num_token++;
            token_len = i;
        }

        // operate symbol
        else if(ch_types[idx] == CH_OPR_SYMBOL){

            // annotation check
            if(code[idx] == '/'){
                if (code[idx+1] == '/'){
                    // find next '\n' and ignore everything
                    // if there's no '\n', it's the end of code.
                    int i = 1;
                    for(; i<(code_length-idx);i++){
                        if(code[idx+i] == '\n'){
                            break;
                        }
                    }
                    col_index = 1;
                    line_index += 1;
                    idx += i+1;
                    continue;
                } // end "//" type annotation
                else if (code[idx+1] == '*'){
                    // find next "*/" and ignore everything
                    int i = 2;
                    for(; i+1<(code_length-idx);i++){
                        col_index++;
                        if(code[idx+i] == '\n'){
                            line_index += 1;
                            col_index = 1;
                        }
                        else if(code[idx+i] == '*'){
                            if(code[idx+i+1] == '/'){
                                break;
                            }        
                        }
                    }
                    if (i+1 == code_length - idx){
                        throw "Error: unclosed annotation";
                    }
                    else{
                        idx += i+2;
                        continue;
                    }
                } // end "//" type annotation
            } // end annotation check

            int i = 0;
            for(; i<(code_length-idx);i++){
                if(ch_types[idx+i] != CH_OPR_SYMBOL){
                    break;
                }
            }
            char* piece = substr(code, code_length, idx, idx+i);

            // judge ++/--
            if((code[idx] == '+' || code[idx] == '-') && i > 0){
                int j = 0;
                for(; j<(code_length-idx);j++){
                    if(ch_types[idx+i+j] != CH_BLANK){
                        break;
                    }
                }
                char latter = code[idx+i+j]; 
                tokens[num_token] = gen_opr_token(piece, i, line_index, col_index, latter);
            }
            else{
                tokens[num_token] = gen_opr_token(piece, i, line_index, col_index, ' ');
            }
            // delete piece;
            num_token++;
            token_len = i;
        }
        
        // punctuation
        else if(ch_types[idx] == CH_PUN_SYMBOL){

            // char const check
            if(code[idx] == '\''){
                if (idx+2 >= code_length){
                    throw "Error: character const error.";
                }
                else if (code[idx+2] != '\''){
                    throw "Error: character const error.";
                }
                char c = code[idx+1];
                tokens[num_token] = gen_digit_token(c, line_index, col_index); 
                col_index += 3;
                idx += 3;
                continue;
            }

            // string check
            else if (code[idx] == '\"'){
                int i = 1;
                for(; i<(code_length-idx);i++){
                    if(code[idx+i] == '\"'){
                        break;
                    }
                }
                if(i == code_length-idx){
                    throw "Error: Unclosed string.";
                }
                char* piece = substr(code, code_length, idx+1, idx+i);
                tokens[num_token] = gen_str_token(piece, i-2, line_index, col_index);
                col_index += i+1;
                idx += i+1;
                continue;
            }
            
            char* piece = substr(code, code_length, idx, idx+1);
            tokens[num_token] = gen_pun_token(piece, line_index, col_index);
            num_token++;
            token_len = 1;
        }

        // blank
        else if(ch_types[idx] == CH_BLANK){
            if (code[idx] == '\n'){
                idx += 1;
                col_index = 1;
                line_index += 1;
                continue;
            }
            idx += 1;
            col_index += 1;
            continue;
        }
        // others
        else {
            throw "Error: Unrecognizable Character.";
        }

        idx += token_len;
        col_index += token_len;

    }

    CUP_TOKEN* res_tokens = new CUP_TOKEN[num_token];
    for(int i = 0; i < num_token; i++){
        res_tokens[i] = tokens[i];
    }
    // delete tokens;
    *token_count = num_token;
    return res_tokens;
} 
