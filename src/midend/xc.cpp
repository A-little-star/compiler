#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "xc.hpp"

static int val_id = 0;
std::unordered_map<value_ptr, int> val_map;

void irDS2Text(const prog_ptr prog, std::ostream &os) {
    // for (int i = 0; i < 1 && i < prog->funcs->len; i ++ ) {
    //     assert(prog->funcs->kind == RISK_FUNCTION);
    //     func_ptr func = (func_ptr)prog->funcs->buffer[i];
    //     printf("fun @main(): i32 {\n");
    //     for (int j = 0; j < 1 && j < func->bbs->len; j ++ ) {
    //         assert(func->bbs->kind == RISK_BASIC_BLOCK);
    //         basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
    //         printf("%%entry:\n");
    //         for (int k = 0; k < 1 && k < bb->insts->len; k ++ ) {
    //             assert(bb->insts->kind == RISK_INSTRUCTION);
    //             value_ptr value = (value_ptr)bb->insts->buffer[i];
    //             assert(value->kind.tag == IR_RETURN);
    //             value_ptr ret_value = value->kind.data.ret.value;
    //             assert(ret_value->kind.tag == IR_INTEGER);
    //             int int_val = ret_value->kind.data.integer.value;
    //             printf("  ret %d\n", int_val);
    //         }
    //     }
    //     printf("}\n");
    // }
    // return "";
    GenCode(prog, os);

}

void GenCode(const prog_ptr prog, std::ostream &os) {
    GenCode(prog->values, os);
    GenCode(prog->funcs, os);
}

void GenCode(const slice_ptr slice, std::ostream &os) {
    // printf("%d\n", slice->len);
    for (size_t i = 0; i < slice->len; i ++ ) {
        auto ptr = slice->buffer[i];
        switch (slice->kind) {
            case RISK_FUNCTION:
                GenCode((func_ptr)ptr, os);
                break;
            case RISK_BASIC_BLOCK:
                GenCode((basic_block_ptr)ptr, os);
                break;
            case RISK_VALUE:
                GenCode((value_ptr)ptr, os);
                break;
            default:
            {
                // std::cout << slice->kind << std::endl;
                assert(false);
            }
        }
    }
}

void GenCode(const func_ptr func, std::ostream &os) {
    os << "fun @" << func->name << "(): i32 {\n";
    GenCode(func->bbs, os);
    os << "}\n";
}

void GenCode(const basic_block_ptr bb, std::ostream &os) {
    os << "%entry:\n";
    GenCode(bb->insts, os);
}

void GenCode(const value_ptr val, std::ostream &os) {
    const auto &kind = val->kind;
    switch (kind.tag) {
        case IR_RETURN:
        {
            if (kind.data.ret.value->kind.tag == IR_INTEGER) {
                os << "  ret " << kind.data.ret.value->kind.data.integer.value << std::endl;
            }
            else if (kind.data.ret.value->kind.tag == IR_BINARY) {
                os << "  ret %" << val_map[kind.data.ret.value] << std::endl;
            }
            else {
                printf("There is a exception in GenCode of value_ptr!\n");
            }
            break;
        }
        case IR_BINARY:
        {
            std::string l_str, r_str, op;
            // 生成第一个操作数的字符串
            if (kind.data.binary.lhs->kind.tag == IR_INTEGER) {
                l_str = std::to_string(kind.data.binary.lhs->kind.data.integer.value);
            }
            else {
                l_str = "%";
                l_str += std::to_string(val_map[kind.data.binary.lhs]);
            }
            // 生成第二个操作数的字符串
            if (kind.data.binary.rhs->kind.tag == IR_INTEGER) {
                r_str = std::to_string(kind.data.binary.rhs->kind.data.integer.value);
            }
            else {
                r_str = "%";
                r_str += std::to_string(val_map[kind.data.binary.rhs]);
            }
            // 生成操作符的字符串
            switch (kind.data.binary.op) {
                case NOT_EQ : op = "neq"; break;
                case EQ : op = "eq"; break;
                case GT : op = "gt"; break;
                case LT : op = "lt"; break;
                case GE : op = "ge"; break;
                case LE : op = "le"; break;
                case ADD : op = "add"; break;
                case SUB : op = "sub"; break;
                case MUL : op = "mul"; break;
                case DIV : op = "div"; break;
                case MOD : op = "mod"; break;
                case AND : op = "and"; break;
                case OR : op = "or"; break;
                case XOR : op = "xor"; break;
                case SHL : op = "shl"; break;
                case SHR : op = "shr"; break;
                case SAR : op = "sar"; break;
            }
            os << "  %" << std::to_string(val_id) << " = " << op << " " << l_str << ", " << r_str << std::endl;
            val_map[val] = val_id;
            val_id ++;
            break;
        }
    }
}