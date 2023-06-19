#ifndef SYMTABL_H
#define SYMTABL_H

#include <cstring>
#include <string>
#include <map>
#include <unordered_map>


enum kind_t {CON, VAR};

struct symval{
    // 符号的类型，如i32
    std::string ty;
    // 符号的数值
    int value;
    // 符号是常量或者变量
    kind_t kind;
    // 有无初始值
    bool has_value;
    // 该符号对应的变量指针
    void *val_p;
};

extern std::unordered_map<std::string, symval> symtable;

#endif