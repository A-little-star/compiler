#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <assert.h>
#include "../midend/xc.hpp"
#include "riscv.hpp"

extern std::unordered_map<value_ptr, int> off_map;
extern std::unordered_map<value_ptr, int> arg_off_map;
extern std::unordered_map<func_ptr, int> has_call;
extern int SizeOfType(type_kind ty);
bool func_cur_has_call = false;
std::string func_name;
int offset = 0;

// std::unordered_map<value_ptr, int> reg_map;

void GenRisc(const prog_ptr prog, RiscvProgram *rp);
void GenRisc(const slice_ptr slice, RiscvProgram *rp);
void GenRisc(const func_ptr func, RiscvProgram *rp);
void GenRisc(const basic_block_ptr bb, RiscvProgram *rp);
void GenRisc(const value_ptr val, RiscvProgram *rp);
// void GenRisc_Binary(const value_ptr val, RiscvProgram *rp);

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

void DumpInit(value_ptr v, RiscvGlobalValue *global_value) {
    if (v->kind.tag == IR_INTEGER) {
        RiscvValueInit *init = new RiscvValueInit;
        init->type = RiscvValueInit::word;
        init->value = v->kind.data.integer.value;
        global_value->AddInit(init);
    }
    else if (v->kind.tag == IR_AGGREGATE) {
        for (int i = 0; i < v->kind.data.aggregate.elems->len; i ++ ) {
            value_ptr val = (value_ptr)v->kind.data.aggregate.elems->buffer[i];
            DumpInit(val, global_value);
        }
    }
    else {
        assert(false);
    }
}

void GenRisc(const value_ptr val, RiscvProgram *rp) {
    const auto &kind = val->kind;
    switch (kind.tag) {
        case IR_ALLOC:
            rp->AddAlloc(val);
            break;
        case IR_GLOBAL_ALLOC: {
            RiscvGlobalValue *global_value = new RiscvGlobalValue;
            global_value->name = val->name.substr(1);
            if (val->kind.data.global_alloc.init->kind.tag == IR_INTEGER) {
                RiscvValueInit *init = new RiscvValueInit;
                int value = val->kind.data.global_alloc.init->kind.data.integer.value;
                if (!value) {
                    init->type = RiscvValueInit::zero;
                    init->value = 4;
                }
                else {
                    init->type = RiscvValueInit::word;
                    init->value = value;
                }
                global_value->AddInit(init);
            }
            else if (val->kind.data.global_alloc.init->kind.tag == IR_ZERO_INIT) {
                RiscvValueInit *init = new RiscvValueInit;
                init->type = RiscvValueInit::zero;
                init->value = SizeOfType(*val->ty.data.pointer.base);
                global_value->AddInit(init);
            }
            else {
                if (val->kind.data.global_alloc.init->kind.data.aggregate.elems == NULL) {
                    RiscvValueInit *init = new RiscvValueInit;
                    init->type = RiscvValueInit::zero;
                    init->value = SizeOfType(*val->ty.data.pointer.base);
                    global_value->AddInit(init);
                }
                else {
                    DumpInit(val->kind.data.global_alloc.init, global_value);
                }
            }
            rp->AddGlobalValue(global_value);
            break;
        }
        case IR_LOAD:
            rp->AddLoad(val);
            break;
        case IR_STORE:
            rp->AddStore(val);
            break;
        case IR_BRANCH:
            rp->AddBranch(val);
            break;
        case IR_JUMP:
            rp->AddJump(val);
            break;
        case IR_CALL:
            rp->AddCall(val);
            break;
        case IR_RETURN:
            rp->AddRet(val);
            break;
        case IR_BINARY:
            rp->AddBinary(val);
            break;
        case IR_GET_ELEM_PTR:
            rp->AddGetelemptr(val);
            break;
        case IR_GET_PTR:
            rp->AddGetptr(val);
            break;
        default: {
            printf("There is an invalid type in GenRisc of value_ptr!\n");
            assert(false);
        }
    }
    // rp->SpillDirtyRegs(val->liveout);
}