#include <vector>
#include "xc.hpp"

struct IndexTreeNode {
    IndexTreeNode *father_index;
    std::vector<value_ptr> indexs;
};

struct IndexTree {
    IndexTreeNode *root;
    IndexTreeNode *current;
};

extern IndexTree it;