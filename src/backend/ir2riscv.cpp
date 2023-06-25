#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "../midend/xc.hpp"

extern std::unordered_map<value_ptr, int> off_map;
extern std::unordered_map<value_ptr, int> arg_off_map;
extern std::unordered_map<func_ptr, int> has_call;
static bool func_cur_has_call = false;
std::string func_name;
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
    os << "  .data\n";
    GenRisc(prog->values, os);
    os << std::endl;
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
    os << "  .text\n  .globl ";
    func_name = func->name.substr(1);
    os << func_name;
    os << "\n";
    offset = CalStackMem(func);
    printf("%d\n", has_call[func]);
    if (has_call[func]) {
        func_cur_has_call = true;
        GenRisc(func->bbs, os);
        func_cur_has_call = false;
    }
    else 
        GenRisc(func->bbs, os);
}

void GenRisc(const basic_block_ptr bb, std::ostream &os) {
    if (bb->name == "%entry") {
        os << func_name << ":\n";
        // os << "main:\n";
        if (offset != 0) os << "  addi sp, sp, " << std::to_string(-offset) << std::endl;
        if (func_cur_has_call) os << "  sw ra, " << offset - 4 << "(sp)" << std::endl;
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
        case IR_GLOBAL_ALLOC:
        {
            os << "  .globl " << val->name.substr(1) << std::endl;
            os << val->name.substr(1) << ":\n";
            int value = val->kind.data.global_alloc.init->kind.data.integer.value;
            if (!value) os << "  .zero 4\n";
            else os << "  .word " << value << std::endl;
            break;
        }
        case IR_LOAD:
        {
            if (kind.data.load.src->kind.tag == IR_GLOBAL_ALLOC) {
                os << "  la t0, " << kind.data.load.src->name.substr(1) << std::endl;
                os << "  lw t0, 0(t0)\n";
                os << "  sw t0, " << off_map[val] << "(sp)" << std::endl;
                break;
            }
            os << "  lw t0, " << off_map[kind.data.load.src] << "(sp)" << std::endl;
            os << "  sw t0, " << off_map[val] << "(sp)" << std::endl;
            break;
        }
        case IR_STORE:
        {
            switch (kind.data.store.value->kind.tag) {
                case IR_INTEGER: os << "  li t0, " << std::to_string(kind.data.store.value->kind.data.integer.value) << std::endl; break;
                case IR_LOAD:
                case IR_BINARY:
                case IR_CALL: os << "  lw t0, " << std::to_string(off_map[kind.data.store.value]) << "(sp)" << std::endl; break;
                case IR_FUNC_ARG: {
                    if (kind.data.store.value->kind.data.func_arg.index <= 8) {
                        os << "  sw a" << kind.data.store.value->kind.data.func_arg.index - 1 << ", " << off_map[kind.data.store.dest] << "(sp)" << std::endl;
                        return;
                    }
                    else {
                        os << "  lw t0, " << offset + (kind.data.store.value->kind.data.func_arg.index - 9) * 4 << "(sp)" << std::endl;
                        // os << "  lw t0, " << offset + arg_off_map[val] << "(sp)" << std::endl;
                    }
                    break;
                }
                default: assert(false);
            }
            switch (kind.data.store.dest->kind.tag) {
                case IR_GLOBAL_ALLOC: {
                    os << "  la t1, " << kind.data.store.dest->name.substr(1) << std::endl;
                    // os << "  lw t1, 0(t1)\n";
                    os << "  sw t0, 0(t1)\n";
                    break;
                }
                case IR_LOAD:
                case IR_BINARY:
                case IR_ALLOC:
                case IR_CALL: os << "  sw t0, " << std::to_string(off_map[kind.data.store.dest]) << "(sp)" << std::endl; break;
                default: assert(false);
            }
            // os << "  sw t0, " << std::to_string(off_map[kind.data.store.dest]) << "(sp)" << std::endl;
            break;
        }
        case IR_BRANCH:
        {
            switch (kind.data.branch.cond->kind.tag) {
                case IR_INTEGER: os << "  li t0, " << std::to_string(kind.data.branch.cond->kind.data.integer.value) << std::endl; break;
                case IR_BINARY:
                case IR_CALL:
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
        case IR_CALL:
        {
            // break;
            // printf("args number: %d\n", kind.data.call.args->len);
            for (size_t i = 0; i < kind.data.call.args->len; i ++ ) {
                const value_ptr arg = (value_ptr)kind.data.call.args->buffer[i];
                if (i < 8) {
                    switch (arg->kind.tag) {
                        case IR_INTEGER: os << "  li a" << i << ", " << arg->kind.data.integer.value << std::endl; break;
                        case IR_BINARY:
                        case IR_LOAD:
                        case IR_CALL: os << "  lw a" << i << ", " << off_map[arg] << "(sp)" << std::endl; break;
                        case IR_FUNC_ARG: {
                            os << "  lw a" << i << ", " << off_map[arg] << "(sp)" << std::endl; break;
                        }
                        default: assert(false);
                    }
                }
                else {
                    switch (arg->kind.tag) {
                        case IR_INTEGER: {
                            os << "  li t0, " << arg->kind.data.integer.value << std::endl;
                            os << "  sw t0, " << arg_off_map[arg] << "(sp)" << std::endl;
                            break;
                        }
                        case IR_BINARY:
                        case IR_LOAD:
                        case IR_CALL:
                        case IR_FUNC_ARG: {
                            os << "  lw t" << i << ", " << off_map[arg] << "(sp)" << std::endl;
                            os << "  sw t" << i << ", " << arg_off_map[arg] << "(sp)" << std::endl;
                            break;
                        }
                        default: assert(false);
                    }
                }
            }
            os << "  call " << kind.data.call.callee->name.substr(1) << std::endl;
            if (kind.data.call.callee->ty.tag == KOOPA_TYPE_INT32)
                os << "  sw a0, " << off_map[val] << "(sp)" << std::endl;
            break;
        }
        case IR_RETURN:
        {
            if (kind.data.ret.value == NULL) {
                if (func_cur_has_call) {
                    os << "  lw ra, " << offset - 4 << "(sp)" << std::endl;
                }
                if (offset) os << "  addi sp, sp, " << offset << std::endl;
                os << "  ret\n";
                break;
            }
            switch (kind.data.ret.value->kind.tag) {
                case IR_INTEGER: os << "  li a0, " << kind.data.ret.value->kind.data.integer.value << std::endl; break;
                case IR_LOAD:
                case IR_BINARY:
                // case 49:
                case IR_CALL: os << "  lw a0, " << off_map[kind.data.ret.value] << "(sp)" << std::endl; break;
                default: assert(false);
            }
            if (func_cur_has_call) os << "  lw ra, " << offset - 4 << "(sp)" << std::endl;
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
    switch (lhs->kind.tag) {
        case IR_INTEGER: os << "  li t0, " << lhs->kind.data.integer.value << std::endl; break;
        case IR_LOAD:
        case IR_BINARY:
        case IR_CALL: os << "  lw t0, " << off_map[lhs] << "(sp)" << std::endl; break;
        default: assert(false);
    }

    // 准备第二个操作数
    switch (rhs->kind.tag) {
        case IR_INTEGER: os << "  li t1, " << rhs->kind.data.integer.value << std::endl; break;
        case IR_LOAD:
        case IR_BINARY:
        case IR_CALL: os << "  lw t1, " << off_map[rhs] << "(sp)" << std::endl; break;
        default: assert(false);
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