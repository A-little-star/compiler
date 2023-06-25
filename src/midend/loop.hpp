#include "xc.hpp"

struct LoopTreeNode {
    LoopTreeNode *father_loop;
    basic_block_ptr loop_entry;
    basic_block_ptr loop_body;
    basic_block_ptr loop_end;
};

struct LoopTree {
    LoopTreeNode *root;
    LoopTreeNode *current;
};

extern LoopTree lt;