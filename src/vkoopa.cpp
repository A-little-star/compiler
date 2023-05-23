#include <cstring>
#include <cassert>
#include <iostream>
#include <map>
#include "koopa.h"
#include "vkoopa.h"

static int reg_id = 0;
extern int val_id;

std::string
regs[32] = {
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

std::map<koopa_raw_value_t, int> map;

void Ope_Num_Prepare_ii(std::string &res, const koopa_raw_value_t &value, std::string &opnum1, std::string &opnum2)
{
    bool zero1 = false, zero2 = false;
    const auto &kind = value->kind;
    // 操作数准备
    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
    if (!zero1)
    {
        res += "  li ";
        res += regs[reg_id];
        res += ", ";
        res += Visit(kind.data.binary.lhs->kind.data.integer);
        res += "\n";
        opnum1 += regs[reg_id];
    }
    else
        opnum1 = "x0";
    if (!zero2)
    {
        res += "  li ";
        res += regs[reg_id + 1];
        res += ", ";
        res += Visit(kind.data.binary.rhs->kind.data.integer);
        res += "\n";
        opnum2 += regs[reg_id + 1];
    }
    else
        opnum2 = "x0";
}

int Ope_Num_Prepare_ie(std::string &res, const koopa_raw_value_t &value, std::string &opnum1, std::string &opnum2)
{
    bool zero1 = false;
    const auto &kind = value->kind;

    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
    if (!Is_Used_By(value, kind.data.binary.rhs)) res += Visit(kind.data.binary.rhs);
    int r_id = reg_id - 1;
    if (!zero1)
    {
        res += "  li ";
        res += regs[reg_id];
        res += ", ";
        res += Visit(kind.data.binary.lhs->kind.data.integer);
        res += "\n";
        opnum1 += regs[reg_id];
    }
    else
        opnum1 = "x0";

    return r_id;
}

int Ope_Num_Prepare_ei(std::string &res, const koopa_raw_value_t &value, std::string &opnum1, std::string &opnum2)
{
    bool zero2 = false;
    const auto &kind = value->kind;
    // 操作数准备
    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
    if (!Is_Used_By(value, kind.data.binary.lhs)) res += Visit(kind.data.binary.lhs);
    int l_id = reg_id - 1;
    if (!zero2)
    {
        res += "  li ";
        res += regs[reg_id];
        res += ", ";
        res += Visit(kind.data.binary.rhs->kind.data.integer);
        res += "\n";
        opnum2 += regs[reg_id];
    }
    else 
        opnum2 = "x0";

    return l_id;
}

void Gen_Asm(std::string &res, const koopa_raw_value_t &value, std::string &opnum1, std::string &opnum2, std::string op, int k)
{
    map[value] = reg_id;
    const auto &kind = value->kind;
    switch (kind.data.binary.lhs->kind.tag)
    {
        case KOOPA_RVT_INTEGER:
        {
            switch (kind.data.binary.rhs->kind.tag)
            {
                case KOOPA_RVT_INTEGER:
                {
                    // 操作数准备
                    Ope_Num_Prepare_ii(res, value, opnum1, opnum2);

                    // 执行指令
                    res += "  ";
                    res += op;
                    res += " ";
                    res += regs[reg_id];
                    res += ", ";
                    res += opnum1;
                    res += ", ";
                    res += opnum2;
                    res += "\n";

                    if (k == 1)
                    {
                        // 执行snez指令
                        res += "  snez ";
                        res += regs[reg_id];
                        res += ", ";
                        res += regs[reg_id];
                        res += "\n";
                    }
                    else if (k == 2)
                    {
                        // 执行seqz指令
                        res += "  seqz ";
                        res += regs[reg_id];
                        res += ", ";
                        res += regs[reg_id];
                        res += "\n";
                    }

                    // 分配一个reg_id
                    reg_id ++;
                    break;
                }
                case KOOPA_RVT_BINARY:
                {
                    // 操作数准备
                    int r_id = Ope_Num_Prepare_ie(res, value, opnum1, opnum2);

                    // 执行指令
                    res += "  ";
                    res += op;
                    res += " ";
                    res += regs[reg_id];
                    res += ", ";
                    res += opnum1;
                    res += ", ";
                    r_id = map[value->kind.data.binary.rhs];
                    res += regs[r_id];
                    res += "\n";

                    if (k == 1)
                    {
                        // 执行snez指令
                        res += "  snez ";
                        res += regs[reg_id];
                        res += ", ";
                        res += regs[reg_id];
                        res += "\n";
                    }
                    else if (k == 2)
                    {
                        // 执行seqz指令
                        res += "  seqz ";
                        res += regs[reg_id];
                        res += ", ";
                        res += regs[reg_id];
                        res += "\n";
                    }

                    // 分配一个reg_id
                    reg_id ++;
                    break;
                }
            }
            break;
        }
        case KOOPA_RVT_BINARY:
        {
            switch (kind.data.binary.rhs->kind.tag)
            {
                case KOOPA_RVT_INTEGER:
                {
                    // 操作数准备
                    int l_id = Ope_Num_Prepare_ei(res, value, opnum1, opnum2);
                    
                    // 执行指令
                    res += "  ";
                    res += op;
                    res += " ";
                    res += regs[reg_id];
                    res += ", ";
                    l_id = map[value->kind.data.binary.lhs];
                    res += regs[l_id];
                    res += ", ";
                    res += opnum2;
                    res += "\n";

                    if (k == 1)
                    {
                        // 执行snez指令
                        res += "  snez ";
                        res += regs[reg_id];
                        res += ", ";
                        res += regs[reg_id];
                        res += "\n";
                    }
                    else if (k == 2)
                    {
                        // 执行seqz指令
                        res += "  seqz ";
                        res += regs[reg_id];
                        res += ", ";
                        res += regs[reg_id];
                        res += "\n";
                    }

                    // 分配一个reg_id
                    reg_id ++;
                    break;
                }
                case KOOPA_RVT_BINARY:
                {
                    // 将两个操作数准备到寄存器中
                    if (!Is_Used_By(value, kind.data.binary.lhs)) res += Visit(kind.data.binary.lhs);
                    int l_id = reg_id - 1;
                    if (!Is_Used_By(value, kind.data.binary.rhs)) res += Visit(kind.data.binary.rhs);
                    int r_id = reg_id - 2;
                    
                    // 执行指令
                    res += "  ";
                    res += op;
                    res += " ";
                    res += regs[reg_id];
                    res += ", ";
                    l_id = map[value->kind.data.binary.lhs];
                    res += regs[l_id];
                    res += ", ";
                    r_id = map[value->kind.data.binary.rhs];
                    res += regs[r_id];
                    res += "\n";

                    if (k == 1)
                    {
                        // 执行snez指令
                        res += "  snez ";
                        res += regs[reg_id];
                        res += ", ";
                        res += regs[reg_id];
                        res += "\n";
                    }
                    else if (k == 2)
                    {
                        // 执行seqz指令
                        res += "  seqz ";
                        res += regs[reg_id];
                        res += ", ";
                        res += regs[reg_id];
                        res += "\n";
                    }

                    // 分配一个reg_id
                    reg_id ++;
                    break;
                }
            }
            break;
        }
    }
}

std::string Visit(const koopa_raw_program_t &program)
{
    std::string res = "  .text\n";
    // 访问全局变量
    res += Visit(program.values);
    // 访问函数
    res += Visit(program.funcs);
    return res;
}

std::string Visit(const koopa_raw_slice_t &slice)
{
    std::string res;
    for (size_t i = 0; i < slice.len; i ++ )
    {
        auto ptr = slice.buffer[i];
        switch (slice.kind)
        {
            case KOOPA_RSIK_FUNCTION:
                res += Visit(reinterpret_cast<koopa_raw_function_t>(ptr));
                break;
            case KOOPA_RSIK_BASIC_BLOCK:
                res += Visit(reinterpret_cast<koopa_raw_basic_block_t>(ptr));
                break;
            case KOOPA_RSIK_VALUE:
                res += Visit(reinterpret_cast<koopa_raw_value_t>(ptr));
                break;
            default:
                // 暂时不会遇到其他内容
                assert(false);
        }
    }
    return res;
}

// 访问函数
std::string Visit(const koopa_raw_function_t &func)
{
    std::string res = "  .global main\n";
    res += func->name + 1;
    res += ":\n";
    res += Visit(func->bbs);
    return res;
}

// 访问基本块
std::string Visit(const koopa_raw_basic_block_t &bb)
{
    std::string res = Visit(bb->insts);
    return res;
}

bool Is_Used_By(const koopa_raw_value_t &v1, const koopa_raw_value_t &v2)
{
    koopa_raw_slice_t slice = v2->used_by;
    for (size_t i = 0; i < slice.len; i ++ )
    {
        auto ptr = slice.buffer[i];
        if (slice.kind == KOOPA_RSIK_VALUE && reinterpret_cast<koopa_raw_value_t>(ptr) == v1)
            return true;
    }
    return false;
}

// 访问指令、全局变量
std::string Visit(const koopa_raw_value_t &value)
{
    std::string res;
    const auto &kind = value->kind;
    switch (kind.tag)
    {
        case KOOPA_RVT_RETURN:
        {
            if (!Is_Used_By(value, kind.data.ret.value)) res += Visit(kind.data.ret.value);
            if (kind.data.ret.value->kind.tag == KOOPA_RVT_BINARY)
            {
                int id = reg_id - 1;
                res += "  mv a0, ";
                res += regs[id];
                res += "\n";
            }
            else if (kind.data.ret.value->kind.tag == KOOPA_RVT_INTEGER)
            {
                bool zero = false;
                if (kind.data.ret.value->kind.data.integer.value == 0) zero = true;
                if (zero)
                {
                    res += "  mv a0, x0\n";
                }
                else 
                {
                    res += "  li ";
                    res += regs[reg_id];
                    res += ", ";
                    res += Visit(kind.data.ret.value->kind.data.integer);
                    res += "\n";
                    res += "  mv a0, ";
                    res += regs[reg_id];
                    res += "\n";
                }
            }
            res += "  ret";
            break;
        }
        case KOOPA_RVT_INTEGER:
            res += Visit(kind.data.integer);
            break;
        case KOOPA_RVT_BINARY:
        {
            std::string opnum1, opnum2;
            switch (kind.data.binary.op)
            {
                case KOOPA_RBO_NOT_EQ:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "xor", 1);
                    break;
                }
                case KOOPA_RBO_EQ:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "xor", 2);
                    break;
                }
                case KOOPA_RBO_GT:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "sgt", 0);
                    break;
                }
                case KOOPA_RBO_LT:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "slt", 0);
                    break;
                }
                case KOOPA_RBO_GE:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "slt", 2);
                    break;
                }
                case KOOPA_RBO_LE:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "sgt", 2);
                    break;
                }
                case KOOPA_RBO_ADD:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "add", 0);
                    break;
                }
                case KOOPA_RBO_SUB:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "sub", 0);
                    break;
                }
                case KOOPA_RBO_MUL:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "mul", 0);
                    break;
                }
                case KOOPA_RBO_DIV:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "div", 0);
                    break;
                }
                case KOOPA_RBO_MOD:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "rem", 0);
                    break;
                }
                case KOOPA_RBO_AND:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "and", 0);
                    break;
                }
                case KOOPA_RBO_OR:
                {
                    Gen_Asm(res, value, opnum1, opnum2, "or", 0);
                    break;
                }
                case KOOPA_RBO_XOR:
                {

                }
                case KOOPA_RBO_SHL:
                {

                }
                case KOOPA_RBO_SHR:
                {

                }
                case KOOPA_RBO_SAR:
                {

                }
                break;
            }
            break;
        }
        default:
            // 其他类型暂时不考虑
            assert(false);
    }
    return res;
}

std::string Visit(const koopa_raw_return_t &ret)
{
    std::string res;
    res += Visit(ret.value);
    res += '\n';
    return res;
}

std::string Visit(const koopa_raw_integer_t &integer)
{
    return std::to_string(integer.value);
}