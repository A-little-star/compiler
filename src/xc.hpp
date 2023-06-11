#ifndef XC_HPP
#define XC_HPP
#include <iostream>
#include <vector>
typedef unsigned int u32;

enum slice_kind {
    RISK_UNKNOWN = 0,
    RISK_VALUE,
    RISK_FUNCTION,
    RISK_BASIC_BLOCK,
    RISK_INSTRUCTION,
};

// slice 表示一种列表类型，列表中可以装载任何类型的元素，如全局变量、函数定义等
typedef struct {
    // Buffer of slice items.
    // const void **buffer;
    std::vector<void*> buffer;
    // Length of slice.
    u32 len;
    // Kind of slice items.
    slice_kind kind;
} slice;

typedef slice *slice_ptr;

typedef struct {
    // Global values.
    slice values;
    // Function definitions.
    slice funcs;
} program;

typedef program *prog_ptr;

typedef struct {
    // Name of function.
    char *name;
    // Parameters.
    slice params;
    // Basic blocks, empty if is a functino declaration.
    slice bbs;
} function;

typedef function *func_ptr;

typedef struct {
    // Name of basic block, null if no name.
    char *name;
    // Parameters of basic block, will be used in SSA optimistic.
    slice params;
    // Values that this basic block is used by.（暂时不知道有什么用）
    slice used_by;
    // Instructions in this basic block.
    slice insts;
} block;

typedef block *block_ptr;

enum inst_kind_tag {
    INTEGER,
    VARIABLE,
    // 变量存储单元分配
    ALLOC,
    GLOBAL_ALLOC,
    // 加载指令
    LOAD,
    // 存储指令
    STORE,
    // 二进制运算
    BINARY,
    // 分支指令
    BRANCH,
    // 跳转指令
    JUMP,
    // 函数调用
    CALL,
    // 函数返回
    RETURN,
};

enum op_t {
    NOT_EQ,
    EQ,
    GT,
    LT,
    GE,
    LE,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    XOR,
    SHL,
    SHR,
    SAR,
};

typedef struct {
    // operator.
    op_t op;
    // Left-hand side value.
    value_ptr lhs;
    // Right-hand side value.
    value_ptr rhs;
} binary_t;

typedef struct {
    int value;
} integer_t;

typedef struct {
    char* name;
} variable_t;

typedef struct {
    value_ptr src;
} load_t;

typedef struct {
    value_ptr dest;
    value_ptr value;
} store_t;

typedef struct {
    value_ptr value;
} return_t;

typedef struct {
    inst_kind_tag tag;
    union {
        integer_t integer;
        variable_t var;
        load_t load;
        store_t store;
        binary_t binary;
        return_t ret;
    } data;
} inst_kind;

// value 类型可是是一条指令，也可以是一个立即数
typedef struct {
    // 该指令用到的操作数
    slice used;
    // 哪些指令用到该指令的返回值
    slice used_by;
    // 指令(操作数)的类型
    inst_kind kind;
} value;

typedef value *value_ptr;

#endif