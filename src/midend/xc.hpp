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
    // 整型数据
    IR_INTEGER,
    IR_VARIABLE,
    // Aggregate constant.
    IR_AGGREGATE,
    // Function argumemnt reference.
    IR_FUNC_ARG,
    // Basic block argument reference.
    IR_BLOCK_ARG,
    // 变量存储单元分配
    IR_ALLOC,
    IR_GLOBAL_ALLOC,
    // 加载指令
    IR_LOAD,
    // 存储指令
    IR_STORE,
    // 获取数组元素
    IR_GET_ELEM_PTR,
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
    // 32-bit integer.
    KOOPA_TYPE_INT32,
    // Unit(void).
    KOOPA_TYPE_UNIT,
    // Array(with base type and length).
    KOOPA_TYPE_ARRAY,
    // Pointer(with base type).
    KOOPA_TYPE_POINTER,
    // Function(with parameter types and return type).
    KOOPA_TYPE_FUNCTION,
};

typedef struct type_kind {
    type_kind_tag tag;
    union {
        struct {
            struct type_kind *base;
            size_t len;
        } array;
        struct {
            struct type_kind *base;
        } pointer;
        struct {
            slice_ptr params;
            struct type_kind *ret;
        } function;
    } data;
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
    slice_ptr elems;
} aggregate_t;

typedef struct {
    size_t index;
} func_arg_t;

typedef struct {
    size_t index;
} block_arg_t;

typedef struct {
    type_kind ty;
    char *name;
} alloc_t;

typedef struct {
    // Initializer.
    value_ptr init;
} global_alloc_t;

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
    // Source.
    value_ptr src;
    // Index.
    value_ptr index;
} get_elem_ptr_t;

struct basic_block;
typedef basic_block *basic_block_ptr;

typedef struct {
    // Condition.
    value_ptr cond;
    // Target if condition is "True".
    basic_block_ptr true_bb;
    // Target if condition is "False".
    basic_block_ptr false_bb;
    // Arguments of "True" target.
    slice_ptr true_args;
    // Arguments of "False" target.
    slice_ptr false_args;
} branch_t;

typedef struct {
    // Target.
    basic_block_ptr target;
    // Arguments of target.
    slice_ptr args;
} jump_t;

typedef struct function *func_ptr;

typedef struct {
    // Callee.
    func_ptr callee;
    // Arguments.
    slice_ptr args;
} call_t;

typedef struct {
    value_ptr value;
} return_t;

typedef struct {
    inst_kind_tag tag;
    union {
        integer_t integer;
        aggregate_t aggregate;
        func_arg_t func_arg;
        block_arg_t block_arg;
        // variable_t var;
        alloc_t alloc;
        global_alloc_t global_alloc;
        load_t load;
        store_t store;
        get_elem_ptr_t get_elem_ptr;
        binary_t binary;
        branch_t branch;
        jump_t jump;
        call_t call;
        return_t ret;
    } data;
} inst_kind;

// value 类型可是是一条指令，也可以是一个立即数
struct value {
    // 该value的数据类型
    type_kind ty;
    // Name of value, null if no name.
    std::string name;
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

typedef struct function {
    // Type of function's return value.
    type_kind ty;
    // Name of function.
    std::string name;
    // Parameters.
    slice_ptr params;
    // Basic blocks, empty if is a functino declaration.
    slice_ptr bbs;
} function;



struct basic_block {
    // Name of basic block, null if no name.
    std::string name;
    // Parameters of basic block, will be used in SSA optimistic.
    slice_ptr params;
    // Values that this basic block is used by.（暂时不知道有什么用）
    slice_ptr used_by;
    // Instructions in this basic block.
    slice_ptr insts;
};


void irDS2Text(const prog_ptr prog, std::ostream &os);

void FreeMem(prog_ptr prog);

int CalStackMem(const func_ptr func);

void ir2riscv(const prog_ptr prog, std::ostream &os);

#endif