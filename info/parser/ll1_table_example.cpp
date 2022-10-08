#include <stdlib.h>
#include <vector>

#include "utils.h"
#include "parser.h"

int TermCount = 30;
int NonTermCount = 37;
std::vector<CUP_NONT> NonTerminators = {
    "PARAMETERS_N", "PARAMETERS", "BLOCK",
};

const char* right_16[] = {"<type>", "<identifier>"};
const char* right_35[] = {"<for>", "(", "EXPR_N", ";", "EXPR_N", ";", "EXPR_N", ")", "S_OR_B"};

std::vector<CUP_GRAMMAR_EXPR> GrammarExprs = {
    (CUP_GRAMMAR_EXPR){16, "PARAMETER", right_16, 2},
    (CUP_GRAMMAR_EXPR){35, "FOR_STC", right_35, 9},
};

CUP_LT_ITEMS item_1 = {
    {"SENTENCES", 20},
};

CUP_LT LL1Table = {
    {"<continue>", item_1},
};

