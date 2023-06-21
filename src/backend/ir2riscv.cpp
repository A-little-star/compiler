#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "../midend/xc.hpp"

extern std::unordered_map<value_ptr, int> off_map;
static int offset = 0;

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
    os << "  .globl main\n";
    offset = CalStackMem(func);
    GenRisc(func->bbs, os);
}

void GenRisc(const basic_block_ptr bb, std::ostream &os) {
    if (bb->name == "%entry") {
        os << "main:\n";
        os << "  addi sp, sp, " << std::to_string(-offset) << std::endl;
    }
    else {
        os << bb->name.substr(1) << ":\n";
    }
    GenRisc(bb->insts, os);
}

void GenRisc(const value_ptr val, std::ostream &os) {
    const auto &kind = val->kind;
    switch (kind.tag) {
        case IR_ALLOC:
        {
            
            break;
        }
        case IR_LOAD:
        {
            os << "  lw t0, " << std::to_string(off_map[kind.data.load.src]) << "(sp)" << std::endl;
            os << "  sw t0, " << std::to_string(off_map[val]) << "(sp)" << std::endl;
            break;
        }
        case IR_STORE:
        {
            if (kind.data.store.value->kind.tag == IR_INTEGER) {
                os << "  li t0, " << std::to_string(kind.data.store.value->kind.data.integer.value) << std::endl;
            }
            else if (kind.data.store.value->kind.tag == IR_BINARY || kind.data.store.value->kind.tag == IR_LOAD) {
                os << "  lw t0, " << std::to_string(off_map[kind.data.store.value]) << "(sp)" << std::endl;
            }
            else {
                printf("There is an exception in visit of IR_STORE of GenRisc!\n");
                assert(false);
            }
            os << "  sw t0, " << std::to_string(off_map[kind.data.store.dest]) << "(sp)" << std::endl;
            break;
        }
        case IR_BRANCH:
        {
            switch (kind.data.branch.cond->kind.tag) {
                case IR_INTEGER: os << "  li t0, " << std::to_string(kind.data.branch.cond->kind.data.integer.value) << std::endl; break;
                case IR_BINARY:
                case IR_LOAD: os << "  lw t0, " << std::to_string(off_map[kind.data.branch.cond]) << "(sp)" << std::endl; break;
                default: break;
            }
            
            os << "  bnez t0, " << kind.data.branch.true_bb->name.substr(1) << std::endl;
            os << "  j " << kind.data.branch.false_bb->name.substr(1) << std::endl;
            break;
        }
        case IR_JUMP:
        {
            os << "  j " << kind.data.jump.target->name.substr(1) << std::endl;
            break;
        }
        case IR_RETURN:
        {
            if (kind.data.ret.value->kind.tag == IR_INTEGER) {
                os << "  li a0, " << kind.data.ret.value->kind.data.integer.value << std::endl;
            }
            else if (kind.data.ret.value->kind.tag == IR_BINARY || kind.data.ret.value->kind.tag == IR_LOAD) {
                os << "  lw a0, " << std::to_string(off_map[kind.data.ret.value]) << "(sp)" << std::endl;
            }
            else {
                printf("There is a exception in GenRisc of value_ptr!\n");
            }
            os << "  addi sp, sp, " << std::to_string(offset) << std::endl;
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

    // 准备第一个操作数
    if (lhs->kind.tag == IR_INTEGER) {
        os << "  li t0, " << std::to_string(lhs->kind.data.integer.value) << std::endl;
    }
    else if (lhs->kind.tag == IR_BINARY || lhs->kind.tag == IR_LOAD) {
        os << "  lw t0, " << std::to_string(off_map[lhs]) << "(sp)" << std::endl;
    }
    else {
        printf("There is an exception in GenRisc_Binary!\n");
        assert(false);
    }

    // 准备第二个操作数
    if (rhs->kind.tag == IR_INTEGER) {
        os << "  li t1, " << std::to_string(rhs->kind.data.integer.value) << std::endl;
    }
    else if (rhs->kind.tag == IR_BINARY || rhs->kind.tag == IR_LOAD) {
        os << "  lw t1, " << std::to_string(off_map[rhs]) << "(sp)" << std::endl;
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

    os << "  " << op << " t0, t0, t1\n";

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
        os << "  " << op << " t0, t0\n";

    os << "  " << "sw t0, " << std::to_string(off_map[val]) << "(sp)" << std::endl;
}