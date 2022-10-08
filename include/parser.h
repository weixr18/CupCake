#ifndef CUP_PARSER_H
#define CUP_PARSER_H

// #define DEBUG_PARSE

#include <map>
#include <stack>
#include "mtree.h"

struct CUP_GRAMMAR_EXPR {
    int id;
    const char* left;
    const char** right;
    int count;
};

typedef const char* CUP_TMNT; // terminator
typedef const char* CUP_NONT; // non-terminator
typedef int CUP_GRMR_ID; // grammar id

typedef std::pair<CUP_NONT, CUP_GRMR_ID> CUP_LT_ITEM; // LL(1) table item
typedef std::vector<CUP_LT_ITEM> CUP_LT_ITEMS; // LL(1) table items
typedef std::map<CUP_TMNT, CUP_LT_ITEMS> CUP_LT; // LL(1) table

typedef MTree CUP_ST;

struct CUP_ST_ANLYS_STATE {  // ST analysis state
    CUP_GRMR_ID gid;
    int cur_token_id;
    MNode* cur_node;
    int lst_nid;
    std::stack<MNode*> n_stack; // nodes waiting for expanding
};

enum CUP_PARSER_ERROR {
    NO_ERROR,
    NODE_STACK_EMPTY,
    TERMINATOR_MISMATCH,
    NO_MATCH_GRAMMAR,
    SUB_TREE_FAILED,
};

#endif // CUP_PARSER_H