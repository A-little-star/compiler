#include "../midend/xc.hpp"

class RiscvReg {
    public:
    enum {
        zero = 0,// 恒为 0
        ra, // 返回地址
        sp, // 栈指针
        gp, // 全局指针
        tp, // 线程指针
        t0, // 临时、备用链接寄存器
        t1, // 临时寄存器
        t2, // 临时寄存器
        fp, // 保存寄存器/帧指针
        s1, // 保存寄存器
        a0, // 函数参数/返回值
        a1, // 函数参数/返回值
        a2, // 函数参数
        a3, // 函数参数
        a4, // 函数参数
        a5, // 函数参数
        a6, // 函数参数
        a7, // 函数参数
        s2, // 保存寄存器
        s3, // 保存寄存器
        s4, // 保存寄存器
        s5, // 保存寄存器
        s6, // 保存寄存器
        s7, // 保存寄存器
        s8, // 保存寄存器
        s9, // 保存寄存器
        s10, // 保存寄存器
        s11, // 保存寄存器
        t3, // 临时寄存器
        t4, // 临时寄存器
        t5, // 临时寄存器
        t6, // 临时寄存器
        TOTAL_NUM // Total number to registers.
    };
    const char *name; // Register name.
    value_ptr var;    // Associated variable.
    bool dirty;       // Whether it is out of sychronized with the memory.
    bool general;     // Whether it is a general-purpose register.

    RiscvReg(const char *reg_name, bool is_general);
    RiscvReg() {};
};

class RiscvInstr {
    public:
    enum OpCode {
        ADD,
        SUB,
        MUL,
        DIV,
        REM,
        NEG,
        J,
        BEQZ,
        BNEZ,
        RET,
        LW,
        LI,
        LA,
        SW,
        MV,
        XOR,
        NOT,
        SEQZ,
        SNEZ,
        EQU,
        NEQ,
        LEQ,
        LES,
        GEQ,
        GRT,
        SLT,
        SGT,
        LAND,
        LOR,
        AND,
        OR,
        CALL,
        PUSH,
        POP,
        ADDI,
    } op_code;

    RiscvReg *r0, *r1, *r2; // 分别为目的操作数、第一个源操作数、第二个源操作数
    int imm;                // 立即数的偏移量
    std::string label;          // 跳转目标。只对跳转指令有效

    void Dump(std::ostream &os);
};

class RiscvBasicBlock {
    public:
    std::string name;
    std::vector<RiscvInstr*> instr;

    void Dump(std::ostream &os);

    void AddInstr(RiscvInstr *v) {
        instr.push_back(v);
    }
};

class RiscvFunc {
    public:
    std::string name;
    std::vector<RiscvBasicBlock*> bbs;

    void Dump(std::ostream &os);

    void AddBasicBlock(RiscvBasicBlock *v) {
        bbs.push_back(v);
    }
    void AddInstr(RiscvInstr *v) {
        RiscvBasicBlock *bb_cur = bbs[bbs.size() - 1];
        bb_cur->AddInstr(v);
    }
};

class RiscvValueInit {
    public:
    enum {
        zero,
        word,
    } type;
    int value;
    void Dump(std::ostream &os);
};

class RiscvGlobalValue {
    public:
    std::string name;
    std::vector<RiscvValueInit*> init;

    void Dump(std::ostream &os);

    void AddInit(RiscvValueInit *v) {
        init.push_back(v);
    }
};

class RiscvData {
    public:
    std::vector<RiscvGlobalValue*> data;

    void Dump(std::ostream &os);

    void AddGlobalValue(RiscvGlobalValue *v) {
        data.push_back(v);
    }
};

class RiscvProgram {
    public:
    RiscvReg *reg[RiscvReg::TOTAL_NUM];

    RiscvProgram();
    std::vector<RiscvData*> data_segment;
    std::vector<RiscvFunc*> func_segment;

    void Dump(std::ostream &os);

    void AddData(RiscvData *v) {
        data_segment.push_back(v);
    }
    void AddFunc(RiscvFunc *v) {
        func_segment.push_back(v);
    }
    void AddBasicBlock(RiscvBasicBlock *v) {
        RiscvFunc *func_cur = func_segment[func_segment.size() - 1];
        func_cur->AddBasicBlock(v);
    }
    void AddGlobalValue(RiscvGlobalValue *v) {
        RiscvData *data_cur = data_segment[data_segment.size() - 1];
        data_cur->AddGlobalValue(v);
    }
    void AddInstr(RiscvInstr *v) {
        RiscvFunc *func_cur = func_segment[func_segment.size() - 1];
        func_cur->AddInstr(v);
    }
};