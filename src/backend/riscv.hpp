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
        RET,
        LW,
        LI,
        LA,
        SW,
        MOVE,
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
    int i;                  // 立即数的偏移量
    std::string l;          // 跳转目标。只对跳转指令有效
};

class RiscvBasicBlock {
    public:
    std::string name;
    std::vector<RiscvInstr> instr;

    void AddInstr(RiscvInstr v) {
        instr.push_back(v);
    }
};

class RiscvFunc {
    public:
    std::string name;
    std::vector<RiscvBasicBlock> bbs;

    void AddBasicBlock(RiscvBasicBlock v) {
        bbs.push_back(v);
    }
};

class RiscvValueInit {
    public:
    enum {
        zero,
        word,
    } type;
    int value;
};

class RiscvGlobalValue {
    public:
    std::string name;
    std::vector<RiscvValueInit> init;

    void AddInit(RiscvValueInit v) {
        init.push_back(v);
    }
};

class RiscvData {
    public:
    std::vector<RiscvGlobalValue> data;

    void AddData(RiscvGlobalValue v) {
        data.push_back(v);
    }
};

class RiscvProgram {
    private:
    RiscvReg *reg[RiscvReg::TOTAL_NUM];
    RiscvBasicBlock *bb_cur;
    public:
    RiscvProgram();
    std::vector<RiscvData> data_segment;
    std::vector<RiscvFunc> func_segment;

    void AddInstr(RiscvInstr v) {
        bb_cur->AddInstr(v);
    }
};