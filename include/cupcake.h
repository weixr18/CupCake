#ifndef CUP_CUPCAKE_H
#define CUP_CUPCAKE_H

#include "utils.h"

CUP_TOKEN* scan(const char code[], CUP_UI code_length, int* token_count);

CUP_ST* parse(CUP_TOKEN* tokens, int token_count);

#endif // CUP_CUPCAKE_H