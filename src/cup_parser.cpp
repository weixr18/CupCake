/*
语法分析
输入token序列
输出抽象语法树ST
*/
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stack>

#include "utils.h"
#include "parser.h"
#include "mtree.h"
#include "cupcake.h"

// global vars
extern int NonTermCount;
extern int TermCount;
extern std::vector<CUP_NONT> NonTerminators;
std::vector<CUP_TMNT> Terminators = {
    "<type>", "<identifier>", "<number>", 
    "<for>", "<while>", "<do>", "<continue>", "<break>",
    "<if>", "<else>", "<switch>", "<case>", "<default>", 
    "<sizeof>", "<return>", 
    "<opr_two>", "<opr_left>", "<opr_right>", "<opr_minus>",
    "(", ")", "[", "]",  "{", "}", ",", ";", "=", ":", "?", 
    "<none>",
};
std::vector<CUP_TMNT> OpenBrackets = {
    "(", "{", "["
};
std::vector<CUP_TMNT> CloseBrackets = {
    ")", "}", "]"
};
extern std::vector<CUP_GRAMMAR_EXPR> GrammarExprs;
extern CUP_LT LL1Table;
std::vector<CUP_GRMR_ID> low_pri_gramr_ids;  // grammars which causes recurrent
std::vector<CUP_GRMR_ID> high_pri_gramr_ids; // grammars which only contains terminators


// calculate different grammar priority
void calc_low_and_high_pri_ids(){
    for(auto expr: GrammarExprs){
        CUP_WORD l = expr.left;
        CUP_WORD r0 = expr.right[0];
        if(is_str_equal(l, r0)){
            low_pri_gramr_ids.push_back(expr.id);
        }
    }
    for(auto expr: GrammarExprs){
        bool is_high_priority = true;
        for(int i = 0; i < expr.count; i++){
            if( !is_element_in_vector(expr.right[i], Terminators)){
                is_high_priority = false;
                break;
            }
        }
        if(is_high_priority){
            high_pri_gramr_ids.push_back(expr.id);
        }
    }
}


// add grammar to stack according to priority, and save the context
void add_grammars_to_stack(std::vector<CUP_GRMR_ID>& matched_grammar_ids,
            int cur_token_id, MNode* cur_node, std::stack<MNode*>& n_stack,
            int lst_nid, std::stack<CUP_ST_ANLYS_STATE>& state_stack){
    // add low priority first
    int n = matched_grammar_ids.size();
    for(int i = 0; i < n; i++){
        CUP_GRMR_ID grm_id = matched_grammar_ids[n-1-i]; // inverse push into stack !!!
        if( is_element_in_vector(grm_id, low_pri_gramr_ids)){
            CUP_ST_ANLYS_STATE state = {
                grm_id, cur_token_id, cur_node, lst_nid, n_stack
            };
            state_stack.push(state);
        }
    }
    // then normals
    for(int i = 0; i < n; i++){
        CUP_GRMR_ID grm_id = matched_grammar_ids[n-1-i]; // inverse push into stack !!!
        if( !is_element_in_vector(grm_id, low_pri_gramr_ids) &&
            !is_element_in_vector(grm_id, high_pri_gramr_ids)){
            CUP_ST_ANLYS_STATE state = {
                grm_id, cur_token_id, cur_node, lst_nid, n_stack
            };
            state_stack.push(state);
        }
    }
    // finnaly high priorities
    for(int i = 0; i < n; i++){
        CUP_GRMR_ID grm_id = matched_grammar_ids[n-1-i]; // inverse push into stack !!!
        if( is_element_in_vector(grm_id, high_pri_gramr_ids)){
            CUP_ST_ANLYS_STATE state = {
                grm_id, cur_token_id, cur_node, lst_nid, n_stack
            };
            state_stack.push(state);
        }
    }
}


// search the LL(1) table for items
std::vector<CUP_GRMR_ID> get_ll1_items(CUP_TMNT cur_t, CUP_WORD cur_n_tag){
    // get LL(1) table item list of cur_t
    CUP_LT_ITEMS tb_item_list_of_cur_t;
    for (auto i = LL1Table.begin(); i != LL1Table.end(); ++i) {
        if(is_str_equal(i->first, cur_t)){
            tb_item_list_of_cur_t = i->second;
            break;
        }
    }
    // get LL(1) table items of (cur_t, cur_n_tag)
    std::vector<CUP_GRMR_ID> matched_grammar_ids;
    for(int i = 0; i < tb_item_list_of_cur_t.size(); i++){
        CUP_LT_ITEM tmp = tb_item_list_of_cur_t[i];
        if(is_str_equal(tmp.first, cur_n_tag)){
            matched_grammar_ids.push_back(tmp.second);
        }
    }
    return matched_grammar_ids;
}


// bracket matching
int get_next_bracket_pos(CUP_TMNT* tmnts, int token_count, CUP_TMNT open_bracket){
    
    CUP_TMNT close_bracket;
    if(is_str_equal(open_bracket, "{")){
        close_bracket = "}";
    }
    else if (is_str_equal(open_bracket, "("))
    {
        close_bracket = ")";
    }
    else if (is_str_equal(open_bracket, "["))
    {
        close_bracket = "]";
    }
    else {
        throw "Error: Not a bracket symbol.";
    }

    int count = 1;
    int i = 0;
    for(; i < token_count; i++){
        if(is_str_equal(tmnts[i], open_bracket)){
            count += 1;
        }
        else if (is_str_equal(tmnts[i], close_bracket)){
            count -= 1;
        }
        if(count <= 0){
            break;
        }
    }
    if(count > 0){
        throw "Error: Brackets don't match.";
    }
    return i;
}


// merge the sub_st at node of main_st
void merge_tree(CUP_ST* main_st, MNode* node, CUP_ST* sub_st){
    if(node->tree != main_st){
        throw "Error: Wrong node.";
    }

    // node info copy
    node->children = sub_st->root->children;
    for(int i = 0; i < node->children.size(); i++){
        node->children[i]->parent = node; // important !!!
    }
    node->element = sub_st->root->element;
    node->gid = sub_st->root->gid;

    // recurrent update
    int nid_offset = main_st->getNodeCount() - 1; // sub_st's root is already in main_st
    int sub_node_count = sub_st->getNodeCount();
    main_st->updateSubTree(node, node->start_tid, nid_offset, node->height);
    main_st->addNodeCount(sub_node_count - 1); // sub_st's root is already in main_st
    delete sub_st;
}

// recurrently update node's end_tid 
void update_end_tid(MNode* node, std::stack<MNode*>& n_stack, int token_id){
    MNode* tmp_node = node;
    MNode* tar_node;
    if(!n_stack.empty()){
        tar_node = (n_stack.top())->parent;
    }
    else{
        tar_node = nullptr;
    }
    while(tmp_node != tar_node){
        tmp_node->end_tid = token_id;
        tmp_node = tmp_node->parent;
    }
}


// recurrently tree building
CUP_ST* build_tree(CUP_TOKEN* tokens, int token_count, CUP_NONT root_nt){
    // tree init
    if(!is_element_in_vector(root_nt, NonTerminators)){
        int line = tokens[0].t_pos.line;
        int col = tokens[0].t_pos.column;
        throw_error_msg("Root node is terminator.", line, col);
    }
    // if(is_str_equal(root_nt, "SENTENCES_N")){
    //     printf("23333\n");
    // }

    CUP_ST* st = new CUP_ST();
    CUP_ST_ELEMENT e_root = { false, root_nt, nullptr };
    st->init(e_root);
    // tags
    const char** tmnts = (const char**) malloc(token_count*sizeof(char*)); // terminators
    for(int i = 0; i < token_count; i++){
        tmnts[i] = token_to_grammar_tag(tokens[i]);
    }

    // vars for choosing different grammar branches
    std::stack<CUP_ST_ANLYS_STATE> state_stack; // branch states
    CUP_PARSER_ERROR error_msg = NO_ERROR;
    // vars for tree node growing
    int cur_token_id = 0;
    MNode* cur_node;
    std::stack<MNode*> n_stack; // nodes waiting for expanding
    n_stack.push(st->root);

#ifdef DEBUG_PARSE
    printf("\n\n");
#endif

    // big loop
    while(cur_token_id < token_count){

        // normal procedure
        if(error_msg == NO_ERROR){
#ifdef DEBUG_PARSE
            printf("-------------------\n");
#endif  
            // get new node
            if(n_stack.empty()){
#ifdef DEBUG_PARSE
                printf("Ohh, node stack empty.\n");
#endif  
                error_msg = NODE_STACK_EMPTY;
                continue;
            }
            cur_node = n_stack.top(); // current node
            n_stack.pop();
            cur_node->start_tid = cur_token_id;
            CUP_TMNT cur_t = tmnts[cur_token_id]; // current terminator
            CUP_WORD cur_n_tag = cur_node->element.tag; // current node tag
#ifdef DEBUG_PARSE
            print_node_path(st, cur_node);
            printf("Current token:%s{%d}\n", cur_t, cur_token_id);
#endif

            // terminator node
            if(cur_node->element.isTerminator){
                // <none> in grammar
                if(is_str_equal(cur_n_tag, "<none>")){
                    continue;
                }
                // terminator doesn't match
                if(!is_str_equal(cur_n_tag, cur_t)){
                    error_msg = TERMINATOR_MISMATCH;
                    continue;
                }

                // bracket terminator
                if(is_element_in_vector(cur_node->element.tag, OpenBrackets)){
                    // check next
                    MNode* next_node = n_stack.top();
                    CUP_WORD next_node_tag = next_node->element.tag;
                    if(is_element_in_vector(next_node_tag, NonTerminators)){
                        // get next nodes
                        n_stack.pop();
                        MNode* nenext_node = n_stack.top();
                        n_stack.pop();
                        CUP_WORD nenext_node_tag = nenext_node->element.tag;
                        // find close bracket
                        if(!is_element_in_vector(nenext_node_tag, CloseBrackets)){
                            int line = tokens[cur_token_id].t_pos.line;
                            int col = tokens[cur_token_id].t_pos.column;
                            throw_error_msg("Bracket node grammar error." , line, col);
                        }
                        int close_pos = get_next_bracket_pos(
                            tmnts+cur_token_id+1, token_count-cur_token_id, cur_node->element.tag
                        );
                        int close_tid = cur_token_id + close_pos + 1;
                        nenext_node->start_tid = close_tid; // start tid

                        // build tree
                        CUP_ST* sub_st;
                        try{
                            sub_st = build_tree(tokens+cur_token_id+1, close_pos, next_node_tag);
                        }
                        catch (char const* s){
                            error_msg = SUB_TREE_FAILED;
                            continue;
                        }
                        // merge tree
                        next_node->start_tid = cur_token_id + 1; // start tid
                        merge_tree(st, next_node, sub_st);
                        // update end tid
                        cur_node->end_tid = cur_token_id + 1; // cur
                        next_node->end_tid = cur_token_id + close_pos + 1; // next 
                        update_end_tid(nenext_node, n_stack, close_tid + 1); // nenext
                        // move forward
                        cur_token_id += close_pos + 2;
                        continue;
                    }     
                } // end bracket terminator

                // normal match terminator
                cur_node->element.tmnt_token = tokens+cur_token_id; 
                cur_token_id += 1;
                update_end_tid(cur_node, n_stack, cur_token_id);
                continue;
            } // end terminator node
            else // non-terminator node
            {
                // 1. look up LL(1) table
                std::vector<CUP_GRMR_ID> matched_grammar_ids = get_ll1_items(cur_t, cur_n_tag);
                // 2. judge failure
                if(matched_grammar_ids.size() == 0){
                    error_msg = NO_MATCH_GRAMMAR;
                    continue;
                }
                // 3. add possible grammar branches to stack
                int lst_nid = st->nodeCount - 1;
                add_grammars_to_stack(
                    matched_grammar_ids, cur_token_id, 
                    cur_node, n_stack, lst_nid, state_stack
                );
#ifdef DEBUG_PARSE
                print_grammar_stack(state_stack);
#endif

                // 4. select one grammar branch
                CUP_GRMR_ID gid = state_stack.top().gid;
                state_stack.pop();
                CUP_GRAMMAR_EXPR grammar = GrammarExprs[gid];
                cur_node->gid = gid; // set gid
#ifdef DEBUG_PARSE
                printf("Choose grammar: [%d]\n", gid);
#endif
                // 5. expand the current node
                for(int i = 0; i < grammar.count; i++){
                    CUP_WORD w = grammar.right[i];
                    bool isTmnt = is_element_in_vector(w, Terminators);
                    CUP_ST_ELEMENT e = {isTmnt, w, nullptr};
                    st->putChild(cur_node, e, cur_token_id);
                }
                // 6. push nodes into stack
                int n = cur_node->children.size();
                for(int i = n-1; i >= 0; i--){
                    // inversely push into stack!!
                    n_stack.push(cur_node->children[i]);
                }

            } // end non-terminator node
        } // end not branch_failed
        
        else { // branch_failed
#ifdef DEBUG_PARSE
            printf("\n-------------------\n");
            printf("[Change branch]\n");
#endif
            // 1. pop new branch
            if (state_stack.empty()){
                int line = tokens[cur_token_id].t_pos.line;
                int col = tokens[cur_token_id].t_pos.column;
                throw_error_msg("Syntax error" , line, col);
            }
#ifdef DEBUG_PARSE
            print_grammar_stack(state_stack);
#endif
            CUP_ST_ANLYS_STATE prev_state = state_stack.top();
            state_stack.pop();

            // 2. loop detection
            if(error_msg != NODE_STACK_EMPTY){
                // condition: a) current node is terminator
                //            b) parrent node's grammar is low priority
                bool need_detect = cur_node->element.isTerminator && cur_node->parent != nullptr;
                if(need_detect){
                    CUP_GRMR_ID parent_gid = cur_node->parent->gid;
                    need_detect = is_element_in_vector(parent_gid, low_pri_gramr_ids);
                }
                if(need_detect){
                    // step back until prev_state.cur_node is higher than current.
                    // prev_state.cur_node == cur_node's parent is one possibility
                    while(prev_state.cur_node->height >= cur_node->height){
                        if(!state_stack.empty()){
                            prev_state = state_stack.top();
                            state_stack.pop();
                        }
                        else{
                            int line = tokens[cur_token_id].t_pos.line;
                            int col = tokens[cur_token_id].t_pos.column;
                            throw_error_msg("State stack empty.", line, col);
                        }         
                    } // end pop state loop
                } // end need detect
            } // end error type check

#ifdef DEBUG_PARSE
            print_grammar_stack(state_stack);
#endif

            // 3. context restore
            cur_node = prev_state.cur_node; // cur_node
            cur_token_id = prev_state.cur_token_id; // token
            n_stack = prev_state.n_stack; // stack 
            error_msg = NO_ERROR; // tag
            st->delAfterNodes(prev_state.lst_nid); // ST
            // st->delSubTree(cur_node); 
#ifdef DEBUG_PARSE
            printf("Choose grammar: [%d]\n", prev_state.gid);
            print_node_path(st, cur_node);
            printf("Current token:%s{%d}\n", tmnts[cur_token_id], cur_token_id);
#endif

            // 4. expand the current node
            CUP_GRMR_ID gid = prev_state.gid;
            cur_node->gid = gid;
            CUP_GRAMMAR_EXPR grammar = GrammarExprs[gid];
            for(int i = 0; i < grammar.count; i++){
                CUP_WORD w = grammar.right[i];
                bool isTmnt = is_element_in_vector(w, Terminators);
                CUP_ST_ELEMENT e = {isTmnt, w, nullptr};
                st->putChild(cur_node, e, cur_token_id);
            }
            // 5. push nodes into stack
            int n = cur_node->children.size();
            for(int i = n-1; i >= 0; i--){
                // inversely push into stack!!
                n_stack.push(cur_node->children[i]);
            }
            continue;
        } // end branch_failed
    } // end while loop
#ifdef DEBUG_PARSE
    printf("Tree built.\n\n");
#endif
    return st;
}


/***************************** Prunning **********************************/

std::vector<CUP_GRMR_ID> tr_grmr_ids; // tail recurrent grammars
/*
1 CODE := CODE CODEPART
15 PARAMETERS := PARAMETERS , PARAMETER
23 SENTENCES := SENTENCES SENTENCE
55 CASES := CASES CASE_STC
75 COMMA_EXP := COMMA_EXP , EXPR
*/

// node merging
void node_merge(CUP_ST* st, MNode* node){
    if(node->tree != st){
        return;
    }
    if(!is_element_in_vector(node->gid, tr_grmr_ids)){
        return;
    }
    CUP_NONT parent_tag = node->element.tag;
    std::stack<MNode*> children;
    std::stack<MNode*> parents;
    MNode* cur_node = node;
    while(is_str_equal(cur_node->element.tag, parent_tag)){
        parents.push(cur_node);
        MNode* cur_child;
        int n = cur_node->children.size();
        for(int i = 0; i < n; i++){
            cur_child = cur_node->children[n-i-1];
            if(!is_str_equal(cur_child->element.tag, parent_tag)){
                children.push(cur_child);
            }
        }
        cur_node = cur_node->children[0];
    }

    // create a new parent
    MNode* new_parent = new MNode();
    new_parent->nid = node->nid;
    new_parent->height = node->height; // h[new_parent] = 0
    new_parent->tree = node->tree;
    new_parent->element = node->element;
    new_parent->gid = -2; // avoid prun again.
    new_parent->start_tid = node->start_tid;
    new_parent->end_tid = node->end_tid;

    // children
    MNode* cur_child;
    while(!children.empty()){
        cur_child = children.top();
        children.pop();
        
        // remove old relation
        if(cur_child->parent != nullptr){
            del_element_in_vector(cur_child, cur_child->parent->children);
        }
        // build new relation
        cur_child->parent = new_parent;
        new_parent->children.push_back(cur_child);
    }

    // node's parent
    if(node == st->root){
        st->root = new_parent;
        new_parent->parent = nullptr;
    }
    else{
        replace_element_in_vector(node, new_parent, node->parent->children);
        new_parent->parent = node->parent;
    }
    return ;   
}


// prunning
void tree_node_merge(CUP_ST* st){
    for(int i = 0; i < low_pri_gramr_ids.size(); i++){
        CUP_GRAMMAR_EXPR grammar = GrammarExprs[low_pri_gramr_ids[i]];
        if(!is_str_equal(grammar.left, "EXPR")){
            tr_grmr_ids.push_back(grammar.id);
        }
    }
    bool prun_done = false;
    while (!prun_done){
        prun_done = true;
        MNode* cur_node;
        std::stack<MNode*> stack;
        stack.push(st->root);
        while(!stack.empty()){
            cur_node = stack.top();
            stack.pop();
            MNode* child;
            for(int i = 0; i < cur_node->children.size(); i++){
                child = cur_node->children[i];
                if(!child->element.isTerminator){
                    stack.push(child);
                }
            }
            if(is_element_in_vector(cur_node->gid, tr_grmr_ids)){
                node_merge(st, cur_node);
                prun_done = false;
                break;
            }
        }
    }
    st->updateHeight(st->root, 0);
    st->updateNid();
    return;
}


/***************************** Parser **********************************/

// parser
CUP_ST* parse(CUP_TOKEN* tokens, int token_count){
#ifdef DEBUG_PARSE
    printf("Terminators number: %d\n", Terminators.size());
    printf("Non-Terminators number: %d\n", NonTerminators.size());
    printf("Grammar Expressions number: %d\n", GrammarExprs.size());
    int sum = 0;
    for(auto &item: LL1Table){
        sum += item.second.size();
    }
    printf("LL(1) table size: %d\n", sum);
#endif
    CUP_ST* st;
    calc_low_and_high_pri_ids();
    st = build_tree(tokens, token_count, "CODE");
    tree_node_merge(st);
    return st;
}
