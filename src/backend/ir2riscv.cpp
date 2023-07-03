#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "../midend/xc.hpp"
#include "riscv.hpp"

extern RiscvProgram *riscvprogram;

extern std::unordered_map<value_ptr, int> off_map;
extern std::unordered_map<value_ptr, int> arg_off_map;
extern std::unordered_map<func_ptr, int> has_call;
static bool func_cur_has_call = false;
std::string func_name;
static int offset = 0;

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
    riscvprogram = new RiscvProgram;
    RiscvData *data = new RiscvData;
    riscvprogram->AddData(data);
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
    RiscvFunc *asm_func = new RiscvFunc;
    riscvprogram->AddFunc(asm_func);
    asm_func->name = func->name.substr(1);
    
    os << "  .text\n  .globl ";
    func_name = func->name.substr(1);
    os << func_name;
    os << "\n";
    offset = CalStackMem(func);
    if (has_call[func]) {
        func_cur_has_call = true;
        GenRisc(func->bbs, os);
        func_cur_has_call = false;
    }
    else 
        GenRisc(func->bbs, os);
}

void GenRisc(const basic_block_ptr bb, std::ostream &os) {
    RiscvBasicBlock *asm_basic_block = new RiscvBasicBlock;
    riscvprogram->AddBasicBlock(asm_basic_block);
    if (bb->name == "%entry") {
        os << func_name << ":\n";
        asm_basic_block->name = func_name;
        if (offset != 0) {
            os << "  addi sp, sp, " << std::to_string(-offset) << std::endl;
            RiscvInstr *asm_instr = new RiscvInstr;
            riscvprogram->AddInstr(asm_instr);
            asm_instr->op_code = RiscvInstr::ADDI;
            asm_instr->r0 = riscvprogram->reg[RiscvReg::sp];
            asm_instr->r1 = riscvprogram->reg[RiscvReg::sp];
            asm_instr->imm = -offset;
        }
        if (func_cur_has_call) {
            os << "  sw ra, " << offset - 4 << "(sp)" << std::endl;
            RiscvInstr *asm_instr = new RiscvInstr;
            riscvprogram->AddInstr(asm_instr);
            asm_instr->op_code = RiscvInstr::SW;
            asm_instr->r0 = riscvprogram->reg[RiscvReg::ra];
            asm_instr->r1 = riscvprogram->reg[RiscvReg::sp];
            asm_instr->imm = offset - 4;
        }
    }
    else {
        os << bb->name.substr(1) << ":\n";
        asm_basic_block->name = bb->name.substr(1);
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
            RiscvGlobalValue *global_value = new RiscvGlobalValue;
            global_value->name = val->name.substr(1);
            RiscvValueInit *init = new RiscvValueInit;
            
            os << "  .globl " << val->name.substr(1) << std::endl;
            os << val->name.substr(1) << ":\n";
            int value = val->kind.data.global_alloc.init->kind.data.integer.value;
            if (!value) {
                os << "  .zero 4\n";
                init->type = RiscvValueInit::zero;
                init->value = 4;
            }
            else {
                os << "  .word " << value << std::endl;
                init->type = RiscvValueInit::word;
                init->value = value;
            }

            global_value->AddInit(init);
            riscvprogram->AddGlobalValue(global_value);
            break;
        }
        case IR_LOAD:
        {
            if (kind.data.load.src->kind.tag == IR_GLOBAL_ALLOC) {
                os << "  la t0, " << kind.data.load.src->name.substr(1) << std::endl;
                os << "  lw t0, 0(t0)\n";
                os << "  sw t0, " << off_map[val] << "(sp)" << std::endl;

                RiscvInstr *instr1 = new RiscvInstr;
                riscvprogram->AddInstr(instr1);
                instr1->op_code = RiscvInstr::LA;
                instr1->r0 = riscvprogram->reg[RiscvReg::t0];
                instr1->label = kind.data.load.src->name.substr(1);

                RiscvInstr *instr2 = new RiscvInstr;
                riscvprogram->AddInstr(instr2);
                instr2->op_code = RiscvInstr::LW;
                instr2->r0 = riscvprogram->reg[RiscvReg::t0];
                instr2->r1 = riscvprogram->reg[RiscvReg::t0];
                instr2->imm = 0;

                RiscvInstr *instr3 = new RiscvInstr;
                riscvprogram->AddInstr(instr3);
                instr3->op_code = RiscvInstr::SW;
                instr3->r0 = riscvprogram->reg[RiscvReg::t0];
                instr3->r1 = riscvprogram->reg[RiscvReg::sp];
                instr3->imm = off_map[val];
                break;
            }
            os << "  lw t0, " << off_map[kind.data.load.src] << "(sp)" << std::endl;
            os << "  sw t0, " << off_map[val] << "(sp)" << std::endl;

            RiscvInstr *instr1 = new RiscvInstr;
            riscvprogram->AddInstr(instr1);
            instr1->op_code = RiscvInstr::LW;
            instr1->r0 = riscvprogram->reg[RiscvReg::t0];
            instr1->r1 = riscvprogram->reg[RiscvReg::sp];
            instr1->imm = off_map[kind.data.load.src];

            RiscvInstr *instr2 = new RiscvInstr;
            riscvprogram->AddInstr(instr2);
            instr2->op_code = RiscvInstr::SW;
            instr2->r0 = riscvprogram->reg[RiscvReg::t0];
            instr2->r1 = riscvprogram->reg[RiscvReg::sp];
            instr2->imm = off_map[val];
            break;
        }
        case IR_STORE:
        {
            switch (kind.data.store.value->kind.tag) {
                case IR_INTEGER: {
                    os << "  li t0, " << std::to_string(kind.data.store.value->kind.data.integer.value) << std::endl;

                    RiscvInstr *instr = new RiscvInstr;
                    riscvprogram->AddInstr(instr);
                    instr->op_code = RiscvInstr::LI;
                    instr->r0 = riscvprogram->reg[RiscvReg::t0];
                    instr->imm = kind.data.store.value->kind.data.integer.value;
                    break;
                }
                case IR_LOAD:
                case IR_BINARY:
                case IR_CALL: {
                    os << "  lw t0, " << std::to_string(off_map[kind.data.store.value]) << "(sp)" << std::endl;

                    RiscvInstr *instr = new RiscvInstr;
                    riscvprogram->AddInstr(instr);
                    instr->op_code = RiscvInstr::LW;
                    instr->r0 = riscvprogram->reg[RiscvReg::t0];
                    instr->r1 = riscvprogram->reg[RiscvReg::sp];
                    instr->imm = off_map[kind.data.store.value];
                    break;
                }
                case IR_FUNC_ARG: {
                    if (kind.data.store.value->kind.data.func_arg.index <= 8) {
                        os << "  sw a" << kind.data.store.value->kind.data.func_arg.index - 1 << ", " << off_map[kind.data.store.dest] << "(sp)" << std::endl;
                        

                        RiscvInstr *instr = new RiscvInstr;
                        riscvprogram->AddInstr(instr);
                        instr->op_code = RiscvInstr::SW;
                        instr->r0 = riscvprogram->reg[RiscvReg::a0 + kind.data.store.value->kind.data.func_arg.index - 1];
                        instr->r1 = riscvprogram->reg[RiscvReg::sp];
                        instr->imm = off_map[kind.data.store.dest];
                        return;
                    }
                    else {
                        os << "  lw t0, " << offset + (kind.data.store.value->kind.data.func_arg.index - 9) * 4 << "(sp)" << std::endl;

                        RiscvInstr *instr = new RiscvInstr;
                        riscvprogram->AddInstr(instr);
                        instr->op_code = RiscvInstr::LW;
                        instr->r0 = riscvprogram->reg[RiscvReg::t0];
                        instr->r1 = riscvprogram->reg[RiscvReg::sp];
                        instr->imm = offset + (kind.data.store.value->kind.data.func_arg.index - 9) * 4;
                    }
                    break;
                }
                default: assert(false);
            }
            switch (kind.data.store.dest->kind.tag) {
                case IR_GLOBAL_ALLOC: {
                    os << "  la t1, " << kind.data.store.dest->name.substr(1) << std::endl;
                    os << "  sw t0, 0(t1)\n";

                    RiscvInstr *instr1 = new RiscvInstr;
                    riscvprogram->AddInstr(instr1);
                    instr1->op_code = RiscvInstr::LA;
                    instr1->r0 = riscvprogram->reg[RiscvReg::t1];
                    instr1->label = kind.data.store.dest->name.substr(1);

                    RiscvInstr *instr2 = new RiscvInstr;
                    riscvprogram->AddInstr(instr2);
                    instr2->op_code = RiscvInstr::SW;
                    instr2->r0 = riscvprogram->reg[RiscvReg::t0];
                    instr2->r1 = riscvprogram->reg[RiscvReg::t1];
                    instr2->imm = 0;
                    break;
                }
                case IR_LOAD:
                case IR_BINARY:
                case IR_ALLOC:
                case IR_CALL: {
                    os << "  sw t0, " << off_map[kind.data.store.dest] << "(sp)" << std::endl;

                    RiscvInstr *instr = new RiscvInstr;
                    riscvprogram->AddInstr(instr);
                    instr->op_code = RiscvInstr::SW;
                    instr->r0 = riscvprogram->reg[RiscvReg::t0];
                    instr->r1 = riscvprogram->reg[RiscvReg::sp];
                    instr->imm = off_map[kind.data.store.dest];
                    break;
                }
                default: assert(false);
            }
            break;
        }
        case IR_BRANCH:
        {
            switch (kind.data.branch.cond->kind.tag) {
                case IR_INTEGER: {
                    os << "  li t0, " << kind.data.branch.cond->kind.data.integer.value << std::endl;

                    RiscvInstr *instr = new RiscvInstr;
                    riscvprogram->AddInstr(instr);
                    instr->op_code = RiscvInstr::LI;
                    instr->r0 = riscvprogram->reg[RiscvReg::t0];
                    instr->imm = kind.data.branch.cond->kind.data.integer.value;
                    break;
                }
                case IR_BINARY:
                case IR_CALL:
                case IR_LOAD: {
                    os << "  lw t0, " << off_map[kind.data.branch.cond] << "(sp)" << std::endl;

                    RiscvInstr *instr = new RiscvInstr;
                    riscvprogram->AddInstr(instr);
                    instr->op_code = RiscvInstr::LW;
                    instr->r0 = riscvprogram->reg[RiscvReg::t0];
                    instr->r1 = riscvprogram->reg[RiscvReg::sp];
                    instr->imm = off_map[kind.data.branch.cond];
                    break;
                }
                default: break;
            }
            
            os << "  bnez t0, " << kind.data.branch.true_bb->name.substr(1) << std::endl;
            os << "  j " << kind.data.branch.false_bb->name.substr(1) << std::endl;

            RiscvInstr *instr1 = new RiscvInstr;
            riscvprogram->AddInstr(instr1);
            instr1->op_code = RiscvInstr::BNEZ;
            instr1->r0 = riscvprogram->reg[RiscvReg::t0];
            instr1->label = kind.data.branch.true_bb->name.substr(1);

            RiscvInstr *instr2 = new RiscvInstr;
            riscvprogram->AddInstr(instr2);
            instr2->op_code = RiscvInstr::J;
            instr2->label = kind.data.branch.false_bb->name.substr(1);
            break;
        }
        case IR_JUMP:
        {
            os << "  j " << kind.data.jump.target->name.substr(1) << std::endl;

            RiscvInstr *instr = new RiscvInstr;
            riscvprogram->AddInstr(instr);
            instr->op_code = RiscvInstr::J;
            instr->label = kind.data.jump.target->name.substr(1);
            break;
        }
        case IR_CALL:
        {
            for (size_t i = 0; i < kind.data.call.args->len; i ++ ) {
                const value_ptr arg = (value_ptr)kind.data.call.args->buffer[i];
                if (i < 8) {
                    switch (arg->kind.tag) {
                        case IR_INTEGER: {
                            os << "  li a" << i << ", " << arg->kind.data.integer.value << std::endl;

                            RiscvInstr *instr = new RiscvInstr;
                            riscvprogram->AddInstr(instr);
                            instr->op_code = RiscvInstr::LI;
                            instr->r0 = riscvprogram->reg[RiscvReg::a0 + i];
                            instr->imm = arg->kind.data.integer.value;
                            break;
                        }
                        case IR_BINARY:
                        case IR_LOAD:
                        case IR_CALL: {
                            os << "  lw a" << i << ", " << off_map[arg] << "(sp)" << std::endl;

                            RiscvInstr *instr = new RiscvInstr;
                            riscvprogram->AddInstr(instr);
                            instr->op_code = RiscvInstr::LW;
                            instr->r0 = riscvprogram->reg[RiscvReg::a0 + i];
                            instr->r1 = riscvprogram->reg[RiscvReg::sp];
                            instr->imm = off_map[arg];
                            break;
                        }
                        case IR_FUNC_ARG: {
                            os << "  lw a" << i << ", " << off_map[arg] << "(sp)" << std::endl;

                            RiscvInstr *instr = new RiscvInstr;
                            riscvprogram->AddInstr(instr);
                            instr->op_code = RiscvInstr::LW;
                            instr->r0 = riscvprogram->reg[RiscvReg::a0 + i];
                            instr->r1 = riscvprogram->reg[RiscvReg::sp];
                            instr->imm = off_map[arg];
                            break;
                        }
                        default: assert(false);
                    }
                }
                else {
                    switch (arg->kind.tag) {
                        case IR_INTEGER: {
                            os << "  li t0, " << arg->kind.data.integer.value << std::endl;
                            os << "  sw t0, " << arg_off_map[arg] << "(sp)" << std::endl;

                            RiscvInstr *instr1 = new RiscvInstr;
                            riscvprogram->AddInstr(instr1);
                            instr1->op_code = RiscvInstr::LI;
                            instr1->r0 = riscvprogram->reg[RiscvReg::t0];
                            instr1->imm = arg->kind.data.integer.value;

                            RiscvInstr *instr2 = new RiscvInstr;
                            riscvprogram->AddInstr(instr2);
                            instr2->op_code = RiscvInstr::SW;
                            instr2->r0 = riscvprogram->reg[RiscvReg::t0];
                            instr2->r1 = riscvprogram->reg[RiscvReg::sp];
                            instr2->imm = arg_off_map[arg];
                            break;
                        }
                        case IR_BINARY:
                        case IR_LOAD:
                        case IR_CALL:
                        case IR_FUNC_ARG: {
                            os << "  lw t" << i << ", " << off_map[arg] << "(sp)" << std::endl;
                            os << "  sw t" << i << ", " << arg_off_map[arg] << "(sp)" << std::endl;

                            RiscvInstr *instr1 = new RiscvInstr;
                            riscvprogram->AddInstr(instr1);
                            instr1->op_code = RiscvInstr::LW;
                            instr1->r0 = riscvprogram->reg[RiscvReg::t0];
                            instr1->r1 = riscvprogram->reg[RiscvReg::sp];
                            instr1->imm = off_map[arg];

                            RiscvInstr *instr2 = new RiscvInstr;
                            riscvprogram->AddInstr(instr2);
                            instr2->op_code = RiscvInstr::SW;
                            instr2->r0 = riscvprogram->reg[RiscvReg::t0];
                            instr2->r1 = riscvprogram->reg[RiscvReg::sp];
                            instr2->imm = arg_off_map[arg];
                            break;
                        }
                        default: assert(false);
                    }
                }
            }
            os << "  call " << kind.data.call.callee->name.substr(1) << std::endl;

            RiscvInstr *instr = new RiscvInstr;
            riscvprogram->AddInstr(instr);
            instr->op_code = RiscvInstr::CALL;
            instr->label = kind.data.call.callee->name.substr(1);
            if (kind.data.call.callee->ty.tag == KOOPA_TYPE_INT32) {
                os << "  sw a0, " << off_map[val] << "(sp)" << std::endl;

                RiscvInstr *instr = new RiscvInstr;
                riscvprogram->AddInstr(instr);
                instr->op_code = RiscvInstr::SW;
                instr->r0 = riscvprogram->reg[RiscvReg::a0];
                instr->r1 = riscvprogram->reg[RiscvReg::sp];
                instr->imm = off_map[val];
            }
            break;
        }
        case IR_RETURN:
        {
            if (kind.data.ret.value == NULL) {
                if (func_cur_has_call) {
                    os << "  lw ra, " << offset - 4 << "(sp)" << std::endl;

                    RiscvInstr *instr = new RiscvInstr;
                    riscvprogram->AddInstr(instr);
                    instr->op_code = RiscvInstr::LW;
                    instr->r0 = riscvprogram->reg[RiscvReg::ra];
                    instr->r1 = riscvprogram->reg[RiscvReg::sp];
                    instr->imm = offset - 4;
                }
                if (offset) {
                    os << "  addi sp, sp, " << offset << std::endl;

                    RiscvInstr *instr = new RiscvInstr;
                    riscvprogram->AddInstr(instr);
                    instr->op_code = RiscvInstr::ADDI;
                    instr->r0 = riscvprogram->reg[RiscvReg::sp];
                    instr->r1 = riscvprogram->reg[RiscvReg::sp];
                    instr->imm = offset;
                }
                os << "  ret\n";

                RiscvInstr *instr = new RiscvInstr;
                riscvprogram->AddInstr(instr);
                instr->op_code = RiscvInstr::RET;
                break;
            }
            switch (kind.data.ret.value->kind.tag) {
                case IR_INTEGER: {
                    os << "  li a0, " << kind.data.ret.value->kind.data.integer.value << std::endl;

                    RiscvInstr *instr = new RiscvInstr;
                    riscvprogram->AddInstr(instr);
                    instr->op_code = RiscvInstr::LI;
                    instr->r0 = riscvprogram->reg[RiscvReg::a0];
                    instr->imm = kind.data.ret.value->kind.data.integer.value;
                    break;
                }
                case IR_LOAD:
                case IR_BINARY:
                case IR_CALL: {
                    os << "  lw a0, " << off_map[kind.data.ret.value] << "(sp)" << std::endl;

                    RiscvInstr *instr = new RiscvInstr;
                    riscvprogram->AddInstr(instr);
                    instr->op_code = RiscvInstr::LW;
                    instr->r0 = riscvprogram->reg[RiscvReg::a0];
                    instr->r1 = riscvprogram->reg[RiscvReg::sp];
                    instr->imm = off_map[kind.data.ret.value];
                    break;
                }
                default: assert(false);
            }
            if (func_cur_has_call) {
                os << "  lw ra, " << offset - 4 << "(sp)" << std::endl;

                RiscvInstr *instr = new RiscvInstr;
                riscvprogram->AddInstr(instr);
                instr->op_code = RiscvInstr::LW;
                instr->r0 = riscvprogram->reg[RiscvReg::ra];
                instr->r1 = riscvprogram->reg[RiscvReg::sp];
                instr->imm = offset - 4;
            }
            os << "  addi sp, sp, " << std::to_string(offset) << std::endl;
            os << "  ret\n";

            RiscvInstr *instr1 = new RiscvInstr;
            riscvprogram->AddInstr(instr1);
            instr1->op_code = RiscvInstr::ADDI;
            instr1->r0 = riscvprogram->reg[RiscvReg::sp];
            instr1->r1 = riscvprogram->reg[RiscvReg::sp];
            instr1->imm = offset;

            RiscvInstr *instr2 = new RiscvInstr;
            riscvprogram->AddInstr(instr2);
            instr2->op_code = RiscvInstr::RET;
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
        case IR_INTEGER: {
            os << "  li t0, " << lhs->kind.data.integer.value << std::endl;

            RiscvInstr *instr = new RiscvInstr;
            riscvprogram->AddInstr(instr);
            instr->op_code = RiscvInstr::LI;
            instr->r0 = riscvprogram->reg[RiscvReg::t0];
            instr->imm = lhs->kind.data.integer.value;
            break;
        }
        case IR_LOAD:
        case IR_BINARY:
        case IR_CALL: {
            os << "  lw t0, " << off_map[lhs] << "(sp)" << std::endl;

            RiscvInstr *instr = new RiscvInstr;
            riscvprogram->AddInstr(instr);
            instr->op_code = RiscvInstr::LW;
            instr->r0 = riscvprogram->reg[RiscvReg::t0];
            instr->r1 = riscvprogram->reg[RiscvReg::sp];
            instr->imm = off_map[lhs];
            break;
        }
        default: assert(false);
    }

    // 准备第二个操作数
    switch (rhs->kind.tag) {
        case IR_INTEGER: {
            os << "  li t1, " << rhs->kind.data.integer.value << std::endl;

            RiscvInstr *instr = new RiscvInstr;
            riscvprogram->AddInstr(instr);
            instr->op_code = RiscvInstr::LI;
            instr->r0 = riscvprogram->reg[RiscvReg::t1];
            instr->imm = rhs->kind.data.integer.value;
            break;
        }
        case IR_LOAD:
        case IR_BINARY:
        case IR_CALL: {
            os << "  lw t1, " << off_map[rhs] << "(sp)" << std::endl;

            RiscvInstr *instr = new RiscvInstr;
            riscvprogram->AddInstr(instr);
            instr->op_code = RiscvInstr::LW;
            instr->r0 = riscvprogram->reg[RiscvReg::t1];
            instr->r1 = riscvprogram->reg[RiscvReg::sp];
            instr->imm = off_map[rhs];
            break;
        }
        default: assert(false);
    }

    RiscvInstr::OpCode opcode;
    // 生成二元运算的主指令
    switch (val->kind.data.binary.op) {
        case NOT_EQ : { op = "xor"; opcode = RiscvInstr::XOR; break; }
        case EQ : { op = "xor"; opcode = RiscvInstr::XOR; break; }
        case GT : { op = "sgt"; opcode = RiscvInstr::SGT; break; }
        case LT : { op = "slt"; opcode = RiscvInstr::SLT; break; }
        case GE : { op = "slt"; opcode = RiscvInstr::SLT; break; }
        case LE : { op = "sgt"; opcode = RiscvInstr::SGT; break; }
        case ADD : { op = "add"; opcode = RiscvInstr::ADD; break; }
        case SUB : { op = "sub"; opcode = RiscvInstr::SUB; break; }
        case MUL : { op = "mul"; opcode = RiscvInstr::MUL; break; }
        case DIV : { op = "div"; opcode = RiscvInstr::DIV; break; }
        case MOD : { op = "rem"; opcode = RiscvInstr::REM; break; }
        case AND : { op = "and"; opcode = RiscvInstr::AND; break; }
        case OR : { op = "or"; opcode = RiscvInstr::OR; break; }
        case XOR : { op = "xor"; opcode = RiscvInstr::XOR; break; }
        // case SHL : { op = "sll"; opcode = RiscvInstr::; break; }
        // case SHR : { op = "srl"; opcode = RiscvInstr::SGT; break; }
        // case SAR : { op = "sra"; opcode = RiscvInstr::SGT; break; }
        default: { printf("op:%d\n", val->kind.data.binary.op); assert(false); }
    }

    os << "  " << op << " t0, t0, t1\n";

    RiscvInstr *instr = new RiscvInstr;
    riscvprogram->AddInstr(instr);
    instr->op_code = opcode;
    instr->r0 = riscvprogram->reg[RiscvReg::t0];
    instr->r1 = riscvprogram->reg[RiscvReg::t0];
    instr->r2 = riscvprogram->reg[RiscvReg::t1];

    // 生成二元运算的副指令
    bool flag = false; // 是否有副指令
    switch (val->kind.data.binary.op) {
        case NOT_EQ : { op = "snez"; flag = true; opcode = RiscvInstr::SNEZ; } break;
        case EQ :
        case GE :
        case LE : { op = "seqz"; flag = true; opcode = RiscvInstr::SEQZ; } break;
        default : break;
    }

    if (flag) {
        os << "  " << op << " t0, t0\n";

        RiscvInstr *instr = new RiscvInstr;
        riscvprogram->AddInstr(instr);
        instr->op_code = opcode;
        instr->r0 = riscvprogram->reg[RiscvReg::t0];
        instr->r1 = riscvprogram->reg[RiscvReg::t0];
    }

    os << "  " << "sw t0, " << off_map[val] << "(sp)" << std::endl;

    RiscvInstr *instr2 = new RiscvInstr;
    riscvprogram->AddInstr(instr2);
    instr2->op_code = RiscvInstr::SW;
    instr2->r0 = riscvprogram->reg[RiscvReg::t0];
    instr2->r1 = riscvprogram->reg[RiscvReg::sp];
    instr2->imm = off_map[val];
}