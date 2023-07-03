#include "riscv.hpp"

RiscvReg::RiscvReg(const char *reg_name, bool is_general) {
    name = reg_name;
    dirty = false;
    var = NULL;
    general = is_general;
}

RiscvProgram::RiscvProgram() {
    bb_cur = NULL;
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