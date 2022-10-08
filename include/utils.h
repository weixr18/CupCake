#ifndef CUP_UTILS_H
#define CUP_UTILS_H

#include <stack>
#include "parser.h"
#include "scanner.h"

typedef unsigned int CUP_UI;

/*** file ***/

char* read_file(const char filename[], int* len);

/*** string ***/

char* substr(const char src[], CUP_UI length, CUP_UI start, CUP_UI end);

bool is_str_equal(const char a[], const char b[], int len);

bool is_str_equal(const char a[], const char b[]);

int count_char_in_str(const char c, const char s[], int len);

void copy_str(const char src[], char* dst, int len);

/*** stl ***/

bool is_element_in_vector(CUP_WORD e, std::vector<CUP_WORD> &v);

bool is_element_in_vector(int e, std::vector<int> &v);

void del_element_in_vector(MNode* e, std::vector<MNode*>& v);

void replace_element_in_vector(MNode* e, MNode* new_e, std::vector<MNode*>& v);

/*** tag ***/

const char* token_to_grammar_tag(CUP_TOKEN& t);

/***************** print *****************/

// node stack [DON'T USE & !!!]
void print_node_stack(std::stack<MNode*> s);

// grammar stack [DON'T USE & !!!]
void print_grammar_stack(std::stack<CUP_ST_ANLYS_STATE> s);

// node path
void print_node_path(CUP_ST* st, MNode* cur_node);

// ST element
void print_element(CUP_ST_ELEMENT* e);

// ST
void print_st(CUP_ST* st);

/*** log ***/

// log token
void log_token(CUP_TOKEN& t);

/*** error ***/
void throw_error_msg(const char* error_type, int line, int col);

#endif //CUP_UTILS_H