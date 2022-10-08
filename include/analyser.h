#ifndef CUP_ANALYSER_H
#define CUP_ANALYSER_H

#include "parser.h"

struct CUP_TYPE_NODE{
    enum CUP_METATYPE {
        BASE,
        ARRAY,
        // POINTER,
    } type;
    union CUP_TYPE_DATA{
        const char* b; // base
        int array_len; // array
    } data;
    CUP_TYPE_NODE* element; // array or pointer
};

#endif // CUP_ANALYSER_H