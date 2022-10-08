#include "mtree.h"
#include "utils.h"

#include <queue>

MTree::~MTree(){
    // TODO
}

void MTree::init(T e) { 
    root = new MNode();

    root->nid = 0;
    root->parent = nullptr;
    root->height = 0; // h[root] = 0
    root->tree = this;
    root->element = e;
    root->gid = -1;
    root->start_tid = -1;
    root->end_tid = -1;

    nodeCount = 1;
}

 
void MTree::putChild(MNode *parent, T e, int start_tid) {
    MNode* child = new MNode();
    parent->children.push_back(child);

    child->nid = nodeCount;
    child->parent = parent;
    child->height = parent->height + 1;
    child->tree = this;
    child->element = e;
    child->gid = -1;
    child->start_tid = -1;
    child->end_tid = -1;
    nodeCount++;
}
 
void MTree::delSubTree(MNode *node){
    // do not delete current node
    if(node->tree != this){
        return;
    }
    for(int i = 0; i < node->children.size(); i++){
        delSubTree(node->children[i]);
    }
    for(int i = 0; i < node->children.size(); i++){
        delete node->children[i];
        nodeCount--;
    }
    node->children.clear();
}

// delete all the nodes which is generated later than node[nid]
void MTree::delAfterNodes(int lst_nid){
    if(lst_nid >= nodeCount){
        return;
    }
    MNode* cur_node;
    std::stack<MNode*> stack;
    stack.push(root);
    while(!stack.empty()){
        cur_node = stack.top();
        stack.pop();
        for(int i = 0; i < cur_node->children.size(); i++){
            MNode* child = cur_node->children[i];
            if(child->nid > lst_nid){
                delSubTree(child); // base
            }
            else{
                stack.push(child); // add to stack
            }
        }
        auto iter = cur_node->children.begin();
        for ( ; iter != cur_node->children.end(); ) {
            if ((*iter)->nid > lst_nid){
                MNode* child = *iter;
                // delete
                delete child;
                nodeCount--;
                // erase
                iter = cur_node->children.erase(iter);
            }
            else{
                iter++;   
            }
        } // end children traverse loop
    } // end stack loop
}

void MTree::updateSubTree(MNode* node, int tid_offset, int nid_offset, int height_offset){
    std::stack<MNode*> stack;
    // do not update node.
    for(int i = 0; i < node->children.size(); i++){
        stack.push(node->children[i]);
    }

    MNode* cur_node;
    while(!stack.empty()){  
        cur_node = stack.top();
        stack.pop();
        for(int i = 0; i < cur_node->children.size(); i++){
            stack.push(cur_node->children[i]);
        }

        cur_node->nid += nid_offset;
        cur_node->height += height_offset;
        cur_node->tree = this;
        cur_node->start_tid += tid_offset;
        cur_node->end_tid += tid_offset;
    }
}

void MTree::updateHeight(MNode* node, int height){
    node->height = height;
    for(int i = 0; i < node->children.size(); i++){
        MNode* child = node->children[i];
        updateHeight(child, height+1);
    }
}

// layer traverse
void MTree::updateNid(){
    std::queue<MNode*> q;
    q.push(root);
    MNode* cur_node;
    int node_count = 0;
    while (!q.empty()){
        cur_node = q.front();
        q.pop();
        cur_node->nid = node_count;
        for(int i = 0; i < cur_node->children.size(); i++){
            q.push(cur_node->children[i]);
        }
    }
    this->nodeCount = node_count;
}

int MTree::getNodeCount(){
    return nodeCount;
}

void MTree::addNodeCount(int n){
    nodeCount += n;
}
