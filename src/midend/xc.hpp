#ifndef XC_HPP
#define XC_HPP
#include <iostream>
#include <vector>
typedef unsigned int u32;

struct value;
typedef struct value value;
typedef value* value_ptr;

enum slice_kind {
    RISK_UNKNOWN = 0,
    RISK_VALUE,
    RISK_FUNCTION,
    RISK_BASIC_BLOCK,
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

enum inst_kind_tag {
    IR_INTEGER,
    IR_VARIABLE,
    // 变量存储单元分配
    IR_ALLOC,
    IR_GLOBAL_ALLOC,
    // 加载指令
    IR_LOAD,
    // 存储指令
    IR_STORE,
    // 二进制运算
    IR_BINARY,
    // 分支指令
    IR_BRANCH,
    // 跳转指令
    IR_JUMP,
    // 函数调用
    IR_CALL,
    // 函数返回
    IR_RETURN,
};

enum type_kind_tag {
    // 
    KOOPA_TYPE_INT32,
    // 
    KOOPA_TYPE_UNIT,
    //
    KOOPA_TYPE_ARRAY,
    //
    KOOPA_TYPE_POINTER,
    //
    KOOPA_TYPE_FUNCTION,
};

typedef struct type_kind {
    type_kind_tag tag;
    // union {
    //     struct {
    //         struct type_kind *base;
    //         size_t len;
    //     } array;
    //     struct {
    //         struct type_kind *base;
    //     } pointer;
    //     struct {
    //         slice_ptr params;
    //         struct type_kind *ret;
    //     } function;
    // } data;
} type_kind;

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
    type_kind ty;
    char *name;
} alloc_t;

// typedef struct {
//     std::string name;
// } variable_t;

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
        // variable_t var;
        alloc_t alloc;
        load_t load;
        store_t store;
        binary_t binary;
        return_t ret;
    } data;
} inst_kind;

// value 类型可是是一条指令，也可以是一个立即数
struct value {
    // 该value的数据类型
    type_kind ty;
    // 该指令用到的操作数
    slice_ptr used;
    // 哪些指令用到该指令的返回值
    slice_ptr used_by;
    // 指令(操作数)的类型
    inst_kind kind;
};

typedef value *value_ptr;

typedef struct {
    // Global values.
    slice_ptr values;
    // Function definitions.
    slice_ptr funcs;
} program;

typedef program *prog_ptr;

typedef struct {
    // Type of function's return value.
    type_kind ty;
    // Name of function.
    std::string name;
    // Parameters.
    slice_ptr params;
    // Basic blocks, empty if is a functino declaration.
    slice_ptr bbs;
} function;

typedef function *func_ptr;

typedef struct {
    // Name of basic block, null if no name.
    char *name;
    // Parameters of basic block, will be used in SSA optimistic.
    slice_ptr params;
    // Values that this basic block is used by.（暂时不知道有什么用）
    slice_ptr used_by;
    // Instructions in this basic block.
    slice_ptr insts;
} basic_block;

typedef basic_block *basic_block_ptr;

void irDS2Text(const prog_ptr prog, std::ostream &os);

void FreeMem(prog_ptr prog);

int CalStackMem(const func_ptr func);

void ir2riscv(const prog_ptr prog, std::ostream &os);

#endif