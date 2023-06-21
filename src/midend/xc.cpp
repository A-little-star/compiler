#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "xc.hpp"

// 函数声明
// IR生成部分
void GenCode(const prog_ptr prog, std::ostream &os);
void GenCode(const slice_ptr slice, std::ostream &os);
void GenCode(const func_ptr func, std::ostream &os);
void GenCode(const basic_block_ptr bb, std::ostream &os);
void GenCode(const value_ptr val, std::ostream &os);
// 释放内存空间部分
void FreeMem(slice_ptr slice);
void FreeMem(func_ptr func);
void FreeMem(basic_block_ptr bb);
void FreeMem(value_ptr val);

static int val_id = 0;
std::unordered_map<value_ptr, int> val_map;

void irDS2Text(const prog_ptr prog, std::ostream &os) {
    printf("The generation of IR code starts now.\n");
    GenCode(prog, os);
}

void GenCode(const prog_ptr prog, std::ostream &os) {
    GenCode(prog->values, os);
    GenCode(prog->funcs, os);
}

void GenCode(const slice_ptr slice, std::ostream &os) {
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
                assert(false);
        }
    }
}

void GenCode(const func_ptr func, std::ostream &os) {
    os << "fun @" << func->name << "(): i32 {\n";
    GenCode(func->bbs, os);
    os << "}\n";
}

void GenCode(const basic_block_ptr bb, std::ostream &os) {
    os << bb->name << ":\n";
    GenCode(bb->insts, os);
}

void GenCode(const value_ptr val, std::ostream &os) {
    const auto &kind = val->kind;
    switch (kind.tag) {
        case IR_ALLOC:
        {
            os << "  " << kind.data.alloc.name << " = alloc ";
            if (kind.data.alloc.ty.tag == KOOPA_TYPE_INT32) {
                os << "i32" << std::endl;
            }
            else {
                assert(false);
            }
            break;
        }
        case IR_LOAD:
        {
            os << "  %" << std::to_string(val_id) << " = load " << kind.data.load.src->kind.data.alloc.name << std::endl;
            val_map[val] = val_id;
            val_id ++;
            break;
        }
        case IR_STORE:
        {
            os << "  store ";
            if (kind.data.store.value->kind.tag == IR_INTEGER) {
                os << std::to_string(kind.data.store.value->kind.data.integer.value) << ", ";
            }
            else {
                os << "%" << std::to_string(val_map[kind.data.store.value]) << ", ";
            }
            os << kind.data.store.dest->kind.data.alloc.name << std::endl;
            break;
        }
        case IR_BRANCH:
        {
            
            os << "  br ";
            switch (kind.data.branch.cond->kind.tag) {
                case IR_INTEGER: os << std::to_string(kind.data.branch.cond->kind.data.integer.value); break;
                case IR_BINARY:
                case IR_LOAD: os << "%" << std::to_string(val_map[kind.data.branch.cond]); break;
                default: break;
            }
            os << ", " << kind.data.branch.true_bb->name << ", " << kind.data.branch.false_bb->name << std::endl;
            
            break;
        }
        case IR_JUMP:
        {
            os << "  jump " << kind.data.jump.target->name << std::endl;
            break;
        }
        case IR_RETURN:
        {
            if (kind.data.ret.value->kind.tag == IR_INTEGER) {
                os << "  ret " << kind.data.ret.value->kind.data.integer.value << std::endl;
            }
            else {//if  //(kind.data.ret.value->kind.tag == IR_BINARY) {
                os << "  ret %" << val_map[kind.data.ret.value] << std::endl;
            }
            // else {
            //     printf("There is a exception in GenCode of value_ptr!\n");
            // }
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
                case NOT_EQ : op = "ne"; break;
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
        default: {
            printf("There is a invalid type in GenCode of value_ptr!\n");
            assert(false);
        }
    }
}


void FreeMem(prog_ptr prog) {
    FreeMem(prog->values);
    FreeMem(prog->funcs);
    delete prog;
    prog = NULL;
}

void FreeMem(slice_ptr slice) {
    for (size_t i = 0; i < slice->len; i ++ ) {
        auto ptr = slice->buffer[i];
        switch (slice->kind) {
            case RISK_FUNCTION:
                FreeMem((func_ptr)ptr);
                break;
            case RISK_BASIC_BLOCK:
                FreeMem((basic_block_ptr)ptr);
                break;
            case RISK_VALUE:
                FreeMem((value_ptr)ptr);
                break;
            default:
                assert(false);
        }
    }
    delete slice;
    slice = NULL;
}

void FreeMem(func_ptr func) {
    FreeMem(func->params);
    FreeMem(func->bbs);
    delete func;
    func = NULL;
}

void FreeMem(basic_block_ptr bb) {
    FreeMem(bb->used_by);
    FreeMem(bb->params);
    FreeMem(bb->insts);
    delete bb;
    bb = NULL;
}

void FreeMem(value_ptr val) {
    auto &kind = val->kind;
    switch (kind.tag) {
        case IR_RETURN:
        {
            if (kind.data.ret.value->kind.tag == IR_INTEGER)
                FreeMem(kind.data.ret.value);
            break;
        }
        case IR_BINARY:
        {
            if (kind.data.binary.lhs->kind.tag == IR_INTEGER)
                FreeMem(kind.data.binary.lhs);
            if (kind.data.binary.rhs->kind.tag == IR_INTEGER)
                FreeMem(kind.data.binary.rhs);
            break;
        }
        default:
            break;
    }
    delete val;
    val = NULL;
}