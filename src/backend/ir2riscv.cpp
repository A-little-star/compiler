#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "../midend/xc.hpp"
#include "riscv.hpp"

extern std::unordered_map<value_ptr, int> off_map;
extern std::unordered_map<value_ptr, int> arg_off_map;
extern std::unordered_map<func_ptr, int> has_call;
static bool func_cur_has_call = false;
std::string func_name;
static int offset = 0;

// std::unordered_map<value_ptr, int> reg_map;

void GenRisc(const prog_ptr prog, RiscvProgram *rp);
void GenRisc(const slice_ptr slice, RiscvProgram *rp);
void GenRisc(const func_ptr func, RiscvProgram *rp);
void GenRisc(const basic_block_ptr bb, RiscvProgram *rp);
void GenRisc(const value_ptr val, RiscvProgram *rp);
void GenRisc_Binary(const value_ptr val, RiscvProgram *rp);

void ir2riscv(const prog_ptr prog, RiscvProgram *rp) {
    GenRisc(prog, rp);
}

void GenRisc(const prog_ptr prog, RiscvProgram *rp) {
    RiscvData *data = new RiscvData;
    rp->AddData(data);
    GenRisc(prog->values, rp);

    GenRisc(prog->funcs, rp);
}

void GenRisc(const slice_ptr slice, RiscvProgram *rp) {
    for (size_t i = 0; i < slice->len; i ++ ) {
        auto ptr = slice->buffer[i];
        switch (slice->kind) {
            case RISK_FUNCTION:
                GenRisc((func_ptr)ptr, rp);
                break;
            case RISK_BASIC_BLOCK:
                GenRisc((basic_block_ptr)ptr, rp);
                break;
            case RISK_VALUE:
                GenRisc((value_ptr)ptr, rp);
                break;
            default:
                assert(false);
        }
    }
}

void GenRisc(const func_ptr func, RiscvProgram *rp) {
    RiscvFunc *asm_func = new RiscvFunc;
    rp->AddFunc(asm_func);
    asm_func->name = func->name.substr(1);
    
    func_name = func->name.substr(1);
    offset = CalStackMem(func);
    if (has_call[func]) {
        func_cur_has_call = true;
        GenRisc(func->bbs, rp);
        func_cur_has_call = false;
    }
    else 
        GenRisc(func->bbs, rp);
}

void GenRisc(const basic_block_ptr bb, RiscvProgram *rp) {
    RiscvBasicBlock *asm_basic_block = new RiscvBasicBlock;
    rp->AddBasicBlock(asm_basic_block);
    if (bb->name == "%entry") {
        asm_basic_block->name = func_name;
        if (offset != 0) {
            RiscvInstr *asm_instr = new RiscvInstr;
            rp->AddInstr(asm_instr);
            asm_instr->op_code = RiscvInstr::ADDI;
            asm_instr->r0 = rp->reg[RiscvReg::sp];
            asm_instr->r1 = rp->reg[RiscvReg::sp];
            asm_instr->imm = -offset;
        }
        if (func_cur_has_call) {
            RiscvInstr *asm_instr = new RiscvInstr;
            rp->AddInstr(asm_instr);
            asm_instr->op_code = RiscvInstr::SW;
            asm_instr->r0 = rp->reg[RiscvReg::ra];
            asm_instr->r1 = rp->reg[RiscvReg::sp];
            asm_instr->imm = offset - 4;
        }
    }
    else {
        asm_basic_block->name = bb->name.substr(1);
    }
    GenRisc(bb->insts, rp);
}

void GenRisc(const value_ptr val, RiscvProgram *rp) {
    const auto &kind = val->kind;
    switch (kind.tag) {
        case IR_ALLOC:
        {
            rp->AddAlloc(val);
            break;
        }
        case IR_GLOBAL_ALLOC:
        {
            assert(false);
            break;
        }
        case IR_LOAD:
        {
            rp->AddLoad(val);
            break;
        }
        case IR_STORE:
        {
            rp->AddStore(val);
            break;
        }
        case IR_BRANCH:
        {
            rp->AddBranch(val);
            break;
        }
        case IR_JUMP:
        {
            rp->AddJump(val);
            break;
        }
        case IR_CALL:
        {
            assert(false);
            break;
        }
        case IR_RETURN:
        {
            if (func_cur_has_call)
                rp->AddInstr(
                    RiscvInstr::LW,
                    rp->reg[RiscvReg::ra],
                    rp->reg[RiscvReg::sp],
                    NULL,
                    offset - 4,
                    ""
                );
            if (offset)
                rp->AddInstr(
                    RiscvInstr::ADDI,
                    rp->reg[RiscvReg::sp],
                    rp->reg[RiscvReg::sp],
                    NULL,
                    offset,
                    ""
                );
            rp->AddRet(val);
            break;
        }
        case IR_BINARY:
        {
            rp->AddBinary(val);
            break;
        }
        default: {
            printf("There is an invalid type in GenRisc of value_ptr!\n");
            assert(false);
        }
    }
}