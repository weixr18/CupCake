#ifndef CUP_TOKEN_H
#define CUP_TOKEN_H

enum CUP_E_OPERATOR_TYPE {
    OPR_TYPE_LEFT,
    OPR_TYPE_RIGHT,
    OPR_TYPE_TWO,
    OPR_TYPE_EQU,
    OPR_TYPE_MINUS,
};

enum CUP_E_NUMBER_TYPE {
    NUM_TYPE_INT,
    NUM_TYPE_FLOAT,
    NUM_TYPE_DOUBLE,
    NUM_TYPE_CHAR,
    NUM_TYPE_LONG,
    NUM_TYPE_SHORT,
    NUM_TYPE_BOOL,
};

union CUP_U_NUMBER {
    int i;
    float f;
    double d;
    char c;
    long l;
    short s;
    bool b;
};

union CUP_U_SUB_CLASS {
    CUP_E_NUMBER_TYPE n;
    CUP_E_OPERATOR_TYPE o;
};

enum CUP_E_TOKEN_CLASS {
    CLS_TYPE,
    CLS_KEYWORD,
    CLS_OPERATOR,
    CLS_PUNCTUATION,
    CLS_IDENTIFIER,
    CLS_NUMBER,
    CLS_STRING,
};


struct CUP_TOKEN {
    CUP_E_TOKEN_CLASS t_class;
    CUP_U_SUB_CLASS t_sub;

    union TOKEN_VALUE {
        const char* t;  // type
        const char* k;  // keyword
        const char* o;  // operator
        const char* p;  // punctuator
        const char* i;  // identifier
        CUP_U_NUMBER n; // number
        const char* s;  // string
    } t_value;

    struct TOKEN_POS{
        int line;
        int column;
    } t_pos;

    int tid; // token id
};

// char CUP_OPR_SYMBOLS[13];
// char CUP_PUN_SYMBOLS[12];

enum CUP_E_CHAR_TYPE {
    CH_CHAR, // a-z A-Z _
    CH_DIGIT, // 0-9
    CH_OPR_SYMBOL, // CUP_OPR_SYMBOLS
    CH_PUN_SYMBOL, // CUP_PUN_SYMBOLS
    CH_BLANK, // \t ' ' \n
    CH_OTHER,
};

typedef const char* CUP_WORD; // terminator or non-terminator

#endif // CUP_TOKEN_H