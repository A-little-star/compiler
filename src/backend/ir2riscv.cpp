#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "../midend/xc.hpp"

std::string regs[31] = {
    "t0",
    "t1",
    "t2",
    "t3",
    "t4",
    "t5",
    "t6",
    "a0",
    "a1",
    "a2",
    "a3",
    "a4",
    "a5",
    "a6",
    "a7"
};

static int reg_id = 0;
std::unordered_map<value_ptr, int> reg_map;

void GenRisc(const prog_ptr prog, std::ostream &os);
void GenRisc(const slice_ptr slice, std::ostream &os);
void GenRisc(const func_ptr func, std::ostream &os);
void GenRisc(const basic_block_ptr bb, std::ostream &os);
void GenRisc(const value_ptr val, std::ostream &os);
void GenRisc_Binary(const value_ptr val, std::ostream &os);

void ir2riscv(const prog_ptr prog, std::ostream &os) {
    GenRisc(prog, os);
}

void GenRisc(const prog_ptr prog, std::ostream &os) {
    GenRisc(prog->values, os);
    os << "  .text\n";
    GenRisc(prog->funcs, os);
}

void GenRisc(const slice_ptr slice, std::ostream &os) {
    for (size_t i = 0; i < slice->len; i ++ ) {
        auto ptr = slice->buffer[i];
        switch (slice->kind) {
            case RISK_FUNCTION:
                GenRisc((func_ptr)ptr, os);
                break;
            case RISK_BASIC_BLOCK:
                GenRisc((basic_block_ptr)ptr, os);
                break;
            case RISK_VALUE:
                GenRisc((value_ptr)ptr, os);
                break;
            default:
                assert(false);
        }
    }
}

void GenRisc(const func_ptr func, std::ostream &os) {
    os << "  .globl main\nmain:\n";
    GenRisc(func->bbs, os);
}

void GenRisc(const basic_block_ptr bb, std::ostream &os) {
    GenRisc(bb->insts, os);
}

void GenRisc(const value_ptr val, std::ostream &os) {
    const auto &kind = val->kind;
    switch (kind.tag) {
        case IR_RETURN:
        {
            if (kind.data.ret.value->kind.tag == IR_INTEGER) {
                os << "  li a0, " << kind.data.ret.value->kind.data.integer.value << std::endl;
            }
            else if (kind.data.ret.value->kind.tag == IR_BINARY) {
                os << "  mv a0, " << regs[reg_map[kind.data.ret.value]] << std::endl;
            }
            else {
                printf("There is a exception in GenRisc of value_ptr!\n");
            }
            os << "  ret\n";
            break;
        }
        case IR_BINARY:
        {
            GenRisc_Binary(val, os);
            break;
        }
        default: {
            printf("There is an invalid type in GenRisc of value_ptr!\n");
            assert(false);
        }
    }
}

void GenRisc_Binary(const value_ptr val, std::ostream &os) {
    value_ptr lhs = val->kind.data.binary.lhs;
    value_ptr rhs = val->kind.data.binary.rhs;
    std::string reg_s1, reg_s2, op;
    int offset = 0;

    // 准备第一个源操作数
    if (lhs->kind.tag == IR_INTEGER) {
        if (lhs->kind.data.integer.value == 0) {
            reg_s1 = "x0";
        }
        else {
            os << "  li " << regs[reg_id + offset] << ", " << std::to_string(lhs->kind.data.integer.value) << std::endl;
            reg_s1 = regs[reg_id + offset];
            offset ++;
        }
    }
    else if (lhs->kind.tag == IR_BINARY) {
        reg_s1 = regs[reg_map[lhs]];
    }
    else {
        printf("There is an exception in GenRisc_Binary!\n");
        assert(false);
    }

    // 准备第二个源操作数
    if (rhs->kind.tag == IR_INTEGER) {
        if (rhs->kind.data.integer.value == 0) {
            reg_s2 = "x0";
        }
        else {
            os << "  li " << regs[reg_id + offset] << ", " << std::to_string(rhs->kind.data.integer.value) << std::endl;
            reg_s2 = regs[reg_id + offset];
            offset ++;
        }
    }
    else if (rhs->kind.tag == IR_BINARY) {
        reg_s2 = regs[reg_map[rhs]];
    }
    else {
        printf("There is an exception in GenRisc_Binary!\n");
        assert(false);
    }

    // 生成二元运算的主指令
    switch (val->kind.data.binary.op) {
        case NOT_EQ : op = "xor"; break;
        case EQ : op = "xor"; break;
        case GT : op = "sgt"; break;
        case LT : op = "slt"; break;
        case GE : op = "slt"; break;
        case LE : op = "sgt"; break;
        case ADD : op = "add"; break;
        case SUB : op = "sub"; break;
        case MUL : op = "mul"; break;
        case DIV : op = "div"; break;
        case MOD : op = "rem"; break;
        case AND : op = "and"; break;
        case OR : op = "or"; break;
        case XOR : op = "xor"; break;
        case SHL : op = "sll"; break;
        case SHR : op = "srl"; break;
        case SAR : op = "sra"; break;
    }

    os << "  " << op << " " << regs[reg_id] << ", " << reg_s1 << ", " << reg_s2 << std::endl;
    reg_map[val] = reg_id;

    // 生成二元运算的副指令
    bool flag = false; // 是否有副指令
    switch (val->kind.data.binary.op) {
        case NOT_EQ : { op = "snez"; flag = true; } break;
        case EQ :
        case GE :
        case LE : { op = "seqz"; flag = true; } break;
        default : break;
    }

    if (flag)
        os << "  " << op << " " << regs[reg_id] << ", " << regs[reg_id] << std::endl;

    reg_id ++;
}