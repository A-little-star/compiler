#ifndef SYMTABL_H
#define SYMTABL_H

#include <cstring>
#include <string>
#include <map>
#include <unordered_map>


enum kind_t {CON, VAR};

enum type_t_tag {I32, ARRAY};

typedef struct type_t {
    type_t_tag tag;
    struct type_t *data;
} type_t;

struct symval{
    // 符号的类型，如i32
    type_t ty;
    // 符号的数值
    int value;
    // 符号是常量或者变量
    kind_t kind;
    // 有无初始值
    bool has_value;
    // 该符号对应的变量指针
    void *val_p;
    // 该符号的重用数
    int use_num;
};

class BlockAST;

struct BlockTreeNode {
    BlockTreeNode *father_block;
    BlockAST *block;
    std::unordered_map<std::string, symval> symtable;
};

struct BlockTree {
    // 根节点
    BlockTreeNode *root;
    // 当前处理的结点
    BlockTreeNode *current;
};

extern BlockTree bt;
// extern std::unordered_map<std::string, symval> symtable;

#endif