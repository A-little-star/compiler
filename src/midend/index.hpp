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

// struct ArrayListTreeNode {
//     ArrayListTreeNode *father_array_list;
//     bool 
// }

extern IndexTree it;