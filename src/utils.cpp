#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <stack>

#include "utils.h"

/*** file ***/

char* read_file(const char filename[], int* len){
    FILE* fp = fopen(filename, "rb"); 
    fseek(fp, 0, SEEK_END); 
    int fileSize = ftell(fp); 
    fseek(fp, 0, SEEK_SET);         
    char* strText = (char*)malloc(fileSize*sizeof(char));
    int n = fread(strText, fileSize, sizeof(char), fp);
    strText[fileSize] = '\0';
    fclose(fp);

    *len = fileSize;
    return strText;
}

/*** string ***/

char* substr(const char src[], CUP_UI length, CUP_UI start, CUP_UI end){
    if(start > length || end > length){
        char* newstr = (char*) malloc(sizeof(char));
        newstr[0] = '\0';
        return newstr;
    }
    int new_len = end - start;
    char* newstr = (char*) malloc(new_len*sizeof(char));
    for(int i = 0; i < new_len; i++){
        newstr[i] = src[start+i];
    }
    newstr[new_len] = '\0'; // important!!!
    return newstr;
}

bool is_str_equal(const char a[], const char b[], int len){
    for(int i = 0; i < len; i++){
        if(a[i] != b[i]){
            return false;
        }
    }
    return true;
}

bool is_str_equal(const char a[], const char b[]){
    return strcmp(a, b) == 0;
}

int count_char_in_str(const char c, const char s[], int len){
    int count = 0;
    for(int i = 0; i < len; i++){
        if(s[i] == c){
            count++;
        }
    }
    return count;
}

void copy_str(const char src[], char* dst, int len){
    for(int i = 0; i < len; i++){
        dst[i] = src[i];
    }
}

/*** stl ***/

bool is_element_in_vector(CUP_WORD e, std::vector<const char *> &v){
    for(auto ve: v){
        if(is_str_equal(e, ve)){
            return true;
        }
    }
	return false;
}

bool is_element_in_vector(int e, std::vector<int> &v){
    for(auto ve: v){
        if(e == ve){
            return true;
        }
    }
	return false;
}


void del_element_in_vector(MNode* e, std::vector<MNode*>& v){
    auto iter = v.begin();
    for ( ; iter != v.end(); ) {
        if ((*iter) == e){
            iter = v.erase(iter);
        }
        else{
            iter++;   
        }
    }
}

void replace_element_in_vector(MNode* e, MNode* new_e, std::vector<MNode*>& v){
    MNode** data = v.data();
    for(int i = 0; i < v.size(); i++){
        if(v[i] == e){
            data[i] = new_e;
        }
    }
}


/*** tag ***/

std::map<const char*, CUP_TMNT> keyword_tag = {
    {"if", "<if>"}, 
    {"do", "<do>"}, 
    {"for", "<for>"},
    {"else", "<else>"},
    {"case", "<case>"},
    {"break", "<break>"},
    {"while", "<while>"},
    {"switch", "<switch>"},
    {"return", "<return>"},
    {"sizeof", "<sizeof>"},
    {"default", "<default>"},
    {"continue", "<continue>"},
};

const char* token_to_grammar_tag(CUP_TOKEN& t){
    if(t.t_class == CLS_KEYWORD){
        CUP_TMNT res;
        for (auto i = keyword_tag.begin(); i != keyword_tag.end(); ++i) {
            if(is_str_equal(i->first, t.t_value.k)){
                res = i->second;
                break;
            }
        }
        return res;
    }
    else if (t.t_class == CLS_OPERATOR){
        if(t.t_sub.o == OPR_TYPE_LEFT){
            return "<opr_left>";
        }
        else if(t.t_sub.o == OPR_TYPE_RIGHT){
            return "<opr_right>";
        }
        else if(t.t_sub.o == OPR_TYPE_TWO){
            return "<opr_two>";
        }
        else if(t.t_sub.o == OPR_TYPE_EQU){
            return "=";
        }
        else if(t.t_sub.o == OPR_TYPE_MINUS){
            return "<opr_minus>";
        }
    }
    else if(t.t_class == CLS_NUMBER){
        return "<number>";
    }
    else if(t.t_class == CLS_PUNCTUATION){
        return t.t_value.p;
    }
    else if(t.t_class == CLS_STRING){
        return "<number>"; 
        // string is actually a [const char *] in C.
        // compiler has a string table which it points to.
    }
    else if(t.t_class == CLS_IDENTIFIER){
        return "<identifier>";
    }
    else if(t.t_class == CLS_TYPE){
        return "<type>";
    }
    return "";
}


/***************** print *****************/

// node stack [DON'T USE & !!!]
void print_node_stack(std::stack<MNode*> s){
    std::stack<MNode*> inverse;
    while(!s.empty()){
        MNode* n = s.top();
        s.pop();
        inverse.push(n);
    }
    while (!inverse.empty()){
        MNode* n = inverse.top();
        inverse.pop();
        printf(n->element.tag);
    }
    printf("\n");
}

// grammar stack [DON'T USE & !!!]
void print_grammar_stack(std::stack<CUP_ST_ANLYS_STATE> s){
    std::stack<CUP_ST_ANLYS_STATE> inverse;
    while(!s.empty()){
        CUP_ST_ANLYS_STATE n = s.top();
        s.pop();
        inverse.push(n);
    }
    printf("Grammar stack: ");
    while (!inverse.empty()){
        CUP_ST_ANLYS_STATE as = inverse.top();
        inverse.pop();
        printf("%d ", as.gid);
    }
    printf("\n");
}

// node path
void print_node_path(CUP_ST* st, MNode* cur_node){
    printf("Tree node count: %d\n", st->getNodeCount());
    std::stack<MNode*> stack;
    MNode* tmp = cur_node;
    while(tmp != nullptr){
        stack.push(tmp);
        tmp = tmp->parent;
    }
    printf("Path: ");
    while(!stack.empty()){
        tmp = stack.top();
        stack.pop();
        printf("%s[%d] -> ", tmp->element.tag, tmp->nid);
    }
    printf("\n");
}

// ST element
void print_element(CUP_ST_ELEMENT* e){
    printf("%s", e->tag);
}

// ST node (recurrent)
void print_node(MNode* node){
    for(int i = 0; i < node->height; i++){
        printf("  ");
    }
    printf("%s\n", node->element.tag);
    // printf("%s[%d/%d]\n", node->element.tag, node->start_tid, node->end_tid);

    /*
    // print token information
    if(node->element.isTerminator){
        printf("%d\n", node->element.tmnt_token->tid);
    }
    else{
        printf("%s\n", node->element.tag);
    }
    */
    
    for(int i = 0; i < node->children.size(); i++){
        print_node(node->children[i]);
    }
}

// ST
void print_st(CUP_ST* st){
    printf("Syntax Tree:\n");
    print_node(st->root);
}

/*** log ***/

// log token
void log_token(CUP_TOKEN& t){
#ifdef DEBUG_SCAN
    int col = t.t_pos.column;
    int line = t.t_pos.line;
    switch (t.t_class)
    {
    case CLS_IDENTIFIER:
        printf("%d\t[IDF]\t%s\tline: %d, column: %d\n", t.tid, t.t_value.i, line, col);
        break;
    case CLS_TYPE:
        printf("%d\t[TYP]\t%s\tline: %d, column: %d\n", t.tid, t.t_value.t, line, col);
        break;
    case CLS_KEYWORD:
        printf("%d\t[KEY]\t%s\tline: %d, column: %d\n", t.tid, t.t_value.k, line, col);
        break;
    case CLS_OPERATOR:
        printf("%d\t[OPR]\t%s\tline: %d, column: %d\n", t.tid, t.t_value.o, line, col);
        break;
    case CLS_PUNCTUATION:
        printf("%d\t[PUN]\t%s\tline: %d, column: %d\n", t.tid, t.t_value.p, line, col);
        break;
    case CLS_NUMBER:
        switch (t.t_sub.n)
        {
        case NUM_TYPE_INT:
            printf("%d\t[NUM]\t%d\tline: %d, column: %d\n", t.tid, t.t_value.n.i, line, col);
            break;
        case NUM_TYPE_DOUBLE:
            printf("%d\t[NUM]\t%lf\tline: %d, column: %d\n", t.tid, t.t_value.n.d, line, col);
            break;
        case NUM_TYPE_CHAR:
            printf("%d\t[NUM]\t%c\tline: %d, column: %d\n", t.tid, t.t_value.n.c, line, col);
            break;
        case NUM_TYPE_BOOL:
            printf("%d\t[NUM]\t%d\tline: %d, column: %d\n", t.tid, t.t_value.n.b, line, col);
            break;
        case NUM_TYPE_FLOAT:
            printf("%d\t[NUM]\t%lf\tline: %d, column: %d\n", t.tid, t.t_value.n.f, line, col);
            break;
        case NUM_TYPE_LONG:
            printf("%d\t[NUM]\t%ld\tline: %d, column: %d\n", t.tid, t.t_value.n.l, line, col);
            break;
        case NUM_TYPE_SHORT:
            printf("%d\t[NUM]\t%d\tline: %d, column: %d\n", t.tid, t.t_value.n.s, line, col);
            break;
        default:
            break;
        }
        break;
    case CLS_STRING:
        printf("%d\t[STR]\t%s\tline: %d, column: %d\n", t.tid, t.t_value.s, line, col);
        break;
    default:
        break;
    }
#endif // DEBUG_SCAN
}

/*** error ***/
void throw_error_msg(const char* error_type, int line, int col){
    char* msg = new char[100];
    sprintf(msg, "Error: %s at %d:%d\0", error_type, line, col);
    throw msg;
}
