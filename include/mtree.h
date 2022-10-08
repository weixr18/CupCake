#ifndef CUP_MTREE_H
#define CUP_MTREE_H

#include <iostream>
#include <vector>

#include "token.h"

typedef struct CUP_ST_ELEMENT {
    bool isTerminator;
    CUP_WORD tag;
    CUP_TOKEN* tmnt_token;
} CUP_ST_ELEMENT;
typedef CUP_ST_ELEMENT T;

class MTree;
typedef struct MNode {
    int nid; // the order of node generation
    std::vector<MNode*> children;
    MNode *parent;
    int height;
    MTree* tree;

    T element;
    int gid;
    int start_tid;
    int end_tid;
} MNode;
 
class MTree {
public:
    ~MTree();
    void init(T element);
    void putChild(MNode *parent, T e, int start_tid);
    void delSubTree(MNode *node); // delete all its children
    void delAfterNodes(int nid);
    void updateSubTree(MNode* node, int tid_offset, int nid_offset, int height_offset);
    void updateHeight(MNode* node, int height);
    void updateNid();
    int getNodeCount();
    void addNodeCount(int n);

    int nodeCount;
    MNode *root;
};

#endif // CUP_MTREE_H