#include "riscv.hpp"
#include <assert.h>

void RiscvProgram::AddAlloc(value_ptr v) {
    GetRegForWrite(v, 0, 0, v->liveout);
}

void RiscvProgram::AddLoad(value_ptr v) {
    int r1 = GetRegForRead(v->kind.data.load.src, 0, v->liveout);
    int r0 = GetRegForWrite(v, r1, 0, v->liveout);
    AddInstr(RiscvInstr::MV, reg[r0], reg[r1], NULL, 0, "");
}

void RiscvProgram::AddStore(value_ptr v) {
    int r1 = GetRegForRead(v->kind.data.store.value, 0, v->liveout);
    int r0 = GetRegForWrite(v->kind.data.store.dest, r1, 0, v->liveout);
    AddInstr(RiscvInstr::MV, reg[r0], reg[r1], NULL, 0, "");
}

void RiscvProgram::AddBinary(value_ptr v) {
    LiveSet liveness = v->liveout;
    liveness.insert(v->kind.data.binary.lhs);
    liveness.insert(v->kind.data.binary.rhs);
    int r1 = GetRegForRead(v->kind.data.binary.lhs, 0, liveness);
    int r2 = GetRegForRead(v->kind.data.binary.rhs, r1, liveness);
    int r0 = GetRegForWrite(v, r1, r2, liveness);
    RiscvInstr::OpCode op;
    switch (v->kind.data.binary.op) {
        case binary_t::NOT_EQ: op = RiscvInstr::XOR; break;
        case binary_t::EQ: op = RiscvInstr::XOR; break;
        case binary_t::GT: op = RiscvInstr::SGT; break;
        case binary_t::LT: op = RiscvInstr::SLT; break;
        case binary_t::GE: op = RiscvInstr::SLT; break;
        case binary_t::LE: op = RiscvInstr::SGT; break;
        case binary_t::ADD: op = RiscvInstr::ADD; break;
        case binary_t::SUB: op = RiscvInstr::SUB; break;
        case binary_t::MUL: op = RiscvInstr::MUL; break;
        case binary_t::DIV: op = RiscvInstr::DIV; break;
        case binary_t::MOD: op = RiscvInstr::REM; break;
        case binary_t::AND: op = RiscvInstr::AND; break;
        case binary_t::OR: op = RiscvInstr::OR; break;
        case binary_t::XOR: op = RiscvInstr::XOR; break;
        default: assert(false);
    }
    AddInstr(op, reg[r0], reg[r1], reg[r2], 0, "");
    switch (v->kind.data.binary.op) {
        case binary_t::NOT_EQ:
            op = RiscvInstr::SNEZ;
            AddInstr(op, reg[r0], reg[r0], NULL, 0, "");
            break;
        case binary_t::EQ:
        case binary_t::GE:
        case binary_t::LE:
            op = RiscvInstr::SEQZ;
            AddInstr(op, reg[r0], reg[r0], NULL, 0, "");
            break;
        default: break;
    }
}

void RiscvProgram::AddBranch(value_ptr v) {
    int r0 = GetRegForRead(v->kind.data.branch.cond, 0, v->liveout);
    AddInstr(RiscvInstr::BNEZ, reg[r0], NULL, NULL, 0, v->kind.data.branch.true_bb->name.substr(1));
    AddInstr(RiscvInstr::J, NULL, NULL, NULL, 0, v->kind.data.branch.false_bb->name.substr(1));
}

void RiscvProgram::AddJump(value_ptr v) {
    AddInstr(RiscvInstr::J, NULL, NULL, NULL, 0, v->kind.data.jump.target->name.substr(1));
}

void RiscvProgram::AddRet(value_ptr v) {
    if (v->kind.data.ret.value == NULL) {
        AddInstr(RiscvInstr::RET, NULL, NULL, NULL, 0, "");
        return;
    }
    int r1 = GetRegForRead(v->kind.data.ret.value, 0, v->liveout);
    AddInstr(RiscvInstr::MV, reg[RiscvReg::a0], reg[r1], NULL, 0, "");
    AddInstr(RiscvInstr::RET, NULL, NULL, NULL, 0, "");
}

RiscvReg::RiscvReg(const char *reg_name, bool is_general) {
    name = reg_name;
    dirty = false;
    var = NULL;
    general = is_general;
}

RiscvProgram::RiscvProgram() {
    reg[RiscvReg::zero] = new RiscvReg("zero", false);
    reg[RiscvReg::ra] = new RiscvReg("ra", false);
    reg[RiscvReg::sp] = new RiscvReg("sp", false);
    reg[RiscvReg::gp] = new RiscvReg("gp", false);
    reg[RiscvReg::tp] = new RiscvReg("tp", false);
    reg[RiscvReg::t0] = new RiscvReg("t0", true);
    reg[RiscvReg::t1] = new RiscvReg("t1", true);
    reg[RiscvReg::t2] = new RiscvReg("t2", true);
    reg[RiscvReg::t3] = new RiscvReg("t3", true);
    reg[RiscvReg::t4] = new RiscvReg("t4", true);
    reg[RiscvReg::t5] = new RiscvReg("t5", true);
    reg[RiscvReg::t6] = new RiscvReg("t6", true);
    reg[RiscvReg::fp] = new RiscvReg("fp", false); // frame pointer
    reg[RiscvReg::s1] = new RiscvReg("s1", true);
    reg[RiscvReg::s2] = new RiscvReg("s2", true);
    reg[RiscvReg::s3] = new RiscvReg("s3", true);
    reg[RiscvReg::s4] = new RiscvReg("s4", true);
    reg[RiscvReg::s5] = new RiscvReg("s5", true);
    reg[RiscvReg::s6] = new RiscvReg("s6", true);
    reg[RiscvReg::s7] = new RiscvReg("s7", true);
    reg[RiscvReg::s8] = new RiscvReg("s8", true);
    reg[RiscvReg::s9] = new RiscvReg("s9", true);
    reg[RiscvReg::s10] = new RiscvReg("s10", true);
    reg[RiscvReg::s11] = new RiscvReg("s11", true);
    reg[RiscvReg::a0] = new RiscvReg("a0", true); // argument, return value
    reg[RiscvReg::a1] = new RiscvReg("a1", true); // argument, return value
    reg[RiscvReg::a2] = new RiscvReg("a2", true); // argument
    reg[RiscvReg::a3] = new RiscvReg("a3", true); // argument
    reg[RiscvReg::a4] = new RiscvReg("a4", true); // argument
    reg[RiscvReg::a5] = new RiscvReg("a5", true); // argument
    reg[RiscvReg::a6] = new RiscvReg("a6", true); // argument
    reg[RiscvReg::a7] = new RiscvReg("a7", true); // argument
}

void RiscvProgram::Dump(std::ostream &os) {
    for (size_t i = 0; i < data_segment.size(); i ++ ) {
        data_segment[i]->Dump(os);
    }
    os << "\n";
    for (size_t i = 0; i < func_segment.size(); i ++ ) {
        func_segment[i]->Dump(os);
    }
}

void RiscvData::Dump(std::ostream &os) {
    os << "  .data\n";
    for (size_t i = 0; i < data.size(); i ++ ) {
        data[i]->Dump(os);
    }
}

void RiscvGlobalValue::Dump(std::ostream &os) {
    os << "  .globl " << name << std::endl << name << ":" << std::endl;
    for (size_t i = 0; i < init.size(); i ++ ) {
        init[i]->Dump(os);
    }
}

void RiscvValueInit::Dump(std::ostream &os) {
    switch (type) {
        case RiscvValueInit::zero: os << "  .zero " << value << std::endl; break;
        case RiscvValueInit::word: os << "  .word " << value << std::endl; break;
        default: assert(false);
    }
}

void RiscvFunc::Dump(std::ostream &os) {
    os << "  .text\n  .globl " << name << std::endl;
    for (size_t i = 0; i < bbs.size(); i ++ ) {
        bbs[i]->Dump(os);
    }
}

void RiscvBasicBlock::Dump(std::ostream &os) {
    os << name << ":\n";
    for (size_t i = 0; i < instr.size(); i ++ ) {
        instr[i]->Dump(os);
    }
}

void RiscvInstr::Dump(std::ostream &os) {
    switch (op_code) {
        case RiscvInstr::ADD: os << "  add " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::SUB: os << "  sub " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::MUL: os << "  mul " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::DIV: os << "  div " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::REM: os << "  rem " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::NEG: os << "  neg " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::J: os << "  j " << label << std::endl; break;
        case RiscvInstr::BEQZ: os << "  beqz " << r0->name << ", " << label << std::endl; break;
        case RiscvInstr::BNEZ: os << "  bnez " << r0->name << ", " << label << std::endl; break;
        case RiscvInstr::RET: os << "  ret " << std::endl; break;
        case RiscvInstr::LW: os << "  lw " << r0->name << ", " << imm << "(" << r1->name << ")" << std::endl; break;
        case RiscvInstr::LI: os << "  li " << r0->name << ", " << imm << std::endl; break;
        case RiscvInstr::LA: os << "  la " << r0->name << ", " << label << std::endl; break;
        case RiscvInstr::SW: os << "  sw " << r0->name << ", " << imm << "(" << r1->name << ")" << std::endl; break;
        case RiscvInstr::MV: os << "  mv " << r0->name << ", " << r1->name << std::endl; break;
        case RiscvInstr::XOR: os << "  xor " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::NOT: assert(false); break;
        case RiscvInstr::SEQZ: os << "  seqz " << r0->name << ", " << r1->name << std::endl; break;
        case RiscvInstr::SNEZ: os << "  snez " << r0->name << ", " << r1->name << std::endl; break;
        case RiscvInstr::EQU: assert(false);
        case RiscvInstr::NEQ: assert(false); break; 
        case RiscvInstr::LEQ: assert(false); break;
        case RiscvInstr::LES: assert(false); break;
        case RiscvInstr::GEQ: assert(false); break;
        case RiscvInstr::GRT: assert(false); break;
        case RiscvInstr::SLT: os << "  slt " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::SGT: os << "  sgt " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::LAND: os << "  land " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::LOR: os << "  lor " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::AND: os << "  and " << r0->name << ", " << r1->name << ", " << r2->name << std::endl;
        case RiscvInstr::OR: os << "  or " << r0->name << ", " << r1->name << ", " << r2->name << std::endl; break;
        case RiscvInstr::CALL: os << "  call " << label << std::endl; break;
        case RiscvInstr::PUSH: os << "  push " << r0->name << std::endl; break;
        case RiscvInstr::POP: os << "  pop " << r0->name << std::endl; break;
        case RiscvInstr::ADDI: os << "  addi " << r0->name << ", " << r1->name << ", " << imm << std::endl; break;
        default: assert(false);
    }
}

int RiscvProgram::GetRegForRead(value_ptr v, int avoid1, LiveSet &live) {
    int i = lookupReg(v);
    if (i < 0) {
        i = lookupReg(NULL);

        if (i < 0) {
            i = selectRegToSpill(avoid1, RiscvReg::zero, live);
            spillReg(i, live);
        }
        if (v->kind.tag != IR_INTEGER) {
            reg[i]->var = v;
            AddInstr(RiscvInstr::LW, reg[i], reg[RiscvReg::sp], NULL, v->offset, "");
        }
        else {
            reg[i]->var = v;
            AddInstr(RiscvInstr::LI, reg[i], NULL, NULL, v->kind.data.integer.value, "");
        }

        reg[i]->dirty = false;
    }
    return i;
}

int RiscvProgram::GetRegForWrite(value_ptr v, int avoid1, int avoid2, LiveSet &live) {
    if (v == NULL || live.count(v) == 0)
        return RiscvReg::zero;

    int i = lookupReg(v);

    if (i < 0) {
        i = lookupReg(NULL);

        if (i < 0) {
            i = selectRegToSpill(avoid1, avoid2, live);
            spillReg(i, live);
        }
        reg[i]->var = v;
    }

    reg[i]->dirty = true;
    return i;
}

int RiscvProgram::lookupReg(value_ptr v) {
    for (int i = 0; i < RiscvReg::TOTAL_NUM; i ++ )
        if (reg[i]->general && reg[i]->var == v)
            return i;

    return -1;
}

int RiscvProgram::selectRegToSpill(int avoid1, int avoid2, LiveSet& live) {
    // 寻找一个已经不活跃的寄存器
    for (int i = 0; i < RiscvReg::TOTAL_NUM; i ++ ) {
        if (!reg[i]->general)
            continue;
        if ((i != avoid1) && (i != avoid2) && live.count(reg[i]->var) == 0)
            return i;
    }

    // 寻找一个clean的寄存器
    for (int i = 0; i < RiscvReg::TOTAL_NUM; i ++ ) {
        if (!reg[i]->general)
            continue;
        if ((i != avoid1) && (i != avoid2) && !reg[i]->dirty)
            return i;
    }

    // 最坏的情况，所有的寄存器中存放的变量都是活跃且dirty的
    int lastUsedReg = 0;
    do {
        lastUsedReg = (lastUsedReg + 1) % RiscvReg::TOTAL_NUM;
    } while ((lastUsedReg == avoid1) || (lastUsedReg == avoid2) || !reg[lastUsedReg]->general);

    return lastUsedReg;
}

void RiscvProgram::spillReg(int i, LiveSet &live) {
    value_ptr v = reg[i]->var;

    if ((v != NULL) && reg[i]->dirty && live.count(v) != 0) {
        AddInstr(RiscvInstr::SW, reg[i], reg[RiscvReg::sp], NULL, v->offset, "");
    }

    reg[i]->var = NULL;
    reg[i]->dirty = false;
}


void RiscvProgram::SpillDirtyRegs(LiveSet &live) {
    int i;

    for (i = 0; i < RiscvReg::TOTAL_NUM; i ++ ) {
        if ((reg[i]->var != NULL) && reg[i]->dirty && live.count(reg[i]->var))
            break;

        reg[i]->var = NULL;
        reg[i]->dirty = false;
    }
    
    if (i < RiscvReg::TOTAL_NUM) {
        for (; i < RiscvReg::TOTAL_NUM; i ++ )
            spillReg(i, live);
    }
}