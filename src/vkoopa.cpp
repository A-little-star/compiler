#include <cstring>
#include <cassert>
#include <iostream>
#include "koopa.h"
#include "vkoopa.h"

static int reg_id = 0;
extern int val_id;

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
            int id = reg_id - 1;
            res += "  mv a0, t";
            res += std::to_string(id);
            res += "\n";
            res += "  ret";
            break;
        }
        case KOOPA_RVT_INTEGER:
            res += Visit(kind.data.integer);
            break;
        case KOOPA_RVT_BINARY:
        {
            bool zero1 = false, zero2 = false;
            std::string opnum1, opnum2;
            // switch(kind.data.binary.lhs->kind.tag)
            // {
            //     case KOOPA_RVT_INTEGER:

            // }
            switch (kind.data.binary.op)
            {
                case KOOPA_RBO_NOT_EQ:
                {
                    switch (kind.data.binary.lhs->kind.tag)
                    {
                        case KOOPA_RVT_INTEGER:
                        {
                            switch (kind.data.binary.rhs->kind.tag)
                            {
                                case KOOPA_RVT_INTEGER:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        if (!zero1) res += std::to_string(reg_id + 1);
                                        else res += std::to_string(reg_id + 1);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        if (!zero1) opnum2 += std::to_string(reg_id + 1);
                                        else opnum2 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum2 = "x0";

                                    // 执行xor指令
                                    res += "  xor t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", ";
                                    res += opnum2;
                                    res += "\n";

                                    // 执行neqz指令
                                    res += "  neqz t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(reg_id);
                                    res += "\n";

                                    // 分配reg_id
                                    reg_id ++;
                                    break;
                                }
                                case KOOPA_RVT_BINARY:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (!Is_Used_By(value, kind.data.binary.rhs)) res += Visit(kind.data.binary.rhs);
                                    int r_id = reg_id - 1;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";

                                    // 执行xor指令
                                    res += "  xor t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

                                    // 执行neqz指令
                                    res += "  neqz t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(reg_id);
                                    res += "\n";

                                    // 分配reg_id
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
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!Is_Used_By(value, kind.data.binary.lhs)) res += Visit(kind.data.binary.lhs);
                                    int l_id = reg_id - 1;
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        opnum2 += std::to_string(reg_id);
                                    }
                                    else 
                                        opnum2 = "x0";

                                    // 执行xor指令
                                    res += "  xor t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum2;
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += "\n";

                                    // 执行neqz指令
                                    res += "  neqz t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(reg_id);
                                    res += "\n";

                                    // 分配reg_id
                                    reg_id ++;
                                    break;
                                }
                                case KOOPA_RVT_BINARY:
                                {
                                    // 操作数准备
                                    if (!Is_Used_By(value, kind.data.binary.lhs)) res += Visit(kind.data.binary.lhs);
                                    int l_id = reg_id - 1;
                                    if (!Is_Used_By(value, kind.data.binary.rhs)) res += Visit(kind.data.binary.rhs);
                                    int r_id = reg_id - 1;

                                    // 执行xor指令
                                    res += "  xor t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

                                    // 执行neqz指令
                                    res += "  neqz t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(reg_id);
                                    res += "\n";

                                    // 分配reg_id
                                    reg_id ++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case KOOPA_RBO_EQ:
                {
                    switch (kind.data.binary.lhs->kind.tag)
                    {
                        case KOOPA_RVT_INTEGER:
                        {
                            switch (kind.data.binary.rhs->kind.tag)
                            {
                                case KOOPA_RVT_INTEGER:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        if (!zero1) res += std::to_string(reg_id + 1);
                                        else res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        if (!zero1) opnum2 += std::to_string(reg_id + 1);
                                        else opnum2 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum2 = "x0";

                                    // 执行xor指令
                                    res += "  xor t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", ";
                                    res += opnum2;
                                    res += "\n";

                                    // 执行seqz指令
                                    res += "  seqz t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(reg_id);
                                    res += "\n";

                                    // 分配reg_id
                                    reg_id ++;
                                    break;
                                }
                                case KOOPA_RVT_BINARY:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (!Is_Used_By(value, kind.data.binary.rhs)) res += Visit(kind.data.binary.rhs);
                                    int r_id = reg_id - 1;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";

                                    // 执行xor指令
                                    res += "  xor t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

                                    // 执行seqz指令
                                    res += "  seqz t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(reg_id);
                                    res += "\n";

                                    // 分配reg_id
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
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!Is_Used_By(value, kind.data.binary.lhs)) res += Visit(kind.data.binary.lhs);
                                    int l_id = reg_id - 1;
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        opnum2 += std::to_string(reg_id);
                                    }
                                    else 
                                        opnum2 = "x0";

                                    // 执行xor指令
                                    res += "  xor t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum2;
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += "\n";

                                    // 执行seqz指令
                                    res += "  seqz t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(reg_id);
                                    res += "\n";

                                    // 分配reg_id
                                    reg_id ++;
                                    break;
                                }
                                case KOOPA_RVT_BINARY:
                                {
                                    // 操作数准备
                                    if (!Is_Used_By(value, kind.data.binary.lhs)) res += Visit(kind.data.binary.lhs);
                                    int l_id = reg_id - 1;
                                    if (!Is_Used_By(value, kind.data.binary.rhs)) res += Visit(kind.data.binary.rhs);
                                    int r_id = reg_id - 1;

                                    // 执行xor指令
                                    res += "  xor t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

                                    // 执行seqz指令
                                    res += "  seqz t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(reg_id);
                                    res += "\n";

                                    // 分配reg_id
                                    reg_id ++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case KOOPA_RBO_GT:
                {

                }
                case KOOPA_RBO_LT:
                {

                }
                case KOOPA_RBO_GE:
                {

                }
                case KOOPA_RBO_LE:
                {

                }
                case KOOPA_RBO_ADD:
                {
                    switch (kind.data.binary.lhs->kind.tag)
                    {
                        case KOOPA_RVT_INTEGER:
                        {
                            switch (kind.data.binary.rhs->kind.tag)
                            {
                                case KOOPA_RVT_INTEGER:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        if (!zero1) res += std::to_string(reg_id + 1);
                                        else res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        if (!zero1) opnum2 += std::to_string(reg_id + 1);
                                        else opnum2 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum2 = "x0";

                                    // 执行add指令
                                    res += "  add t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", ";
                                    res += opnum2;
                                    res += "\n";

                                    // 分配一个reg_id
                                    reg_id ++;
                                    break;
                                }
                                case KOOPA_RVT_BINARY:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (!Is_Used_By(value, kind.data.binary.rhs)) res += Visit(kind.data.binary.rhs);
                                    int r_id = reg_id - 1;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";

                                    // 执行add指令
                                    res += "  add t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

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
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!Is_Used_By(value, kind.data.binary.lhs)) res += Visit(kind.data.binary.lhs);
                                    int l_id = reg_id - 1;
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        opnum2 += std::to_string(reg_id);
                                    }
                                    else 
                                        opnum2 = "x0";
                                    
                                    // 执行add指令
                                    res += "  add t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum2;
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += "\n";

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
                                    int r_id = reg_id - 1;
                                    
                                    // 执行add指令
                                    res += "  add t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

                                    // 分配一个reg_id
                                    reg_id ++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case KOOPA_RBO_SUB:
                {
                    switch (kind.data.binary.lhs->kind.tag)
                    {
                        case KOOPA_RVT_INTEGER:
                        {
                            switch (kind.data.binary.rhs->kind.tag)
                            {
                                case KOOPA_RVT_INTEGER:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        if (!zero1) res += std::to_string(reg_id + 1);
                                        else res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        if (!zero1) opnum2 += std::to_string(reg_id + 1);
                                        else opnum2 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum2 = "x0";

                                    // 执行sub指令
                                    res += "  sub t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", ";
                                    res += opnum2;
                                    res += "\n";

                                    // 分配一个reg_id
                                    reg_id ++;
                                    break;
                                }
                                case KOOPA_RVT_BINARY:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (!Is_Used_By(value, kind.data.binary.rhs)) res += Visit(kind.data.binary.rhs);
                                    int r_id = reg_id - 1;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";

                                    // 执行sub指令
                                    res += "  sub t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

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
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!Is_Used_By(value, kind.data.binary.lhs)) res += Visit(kind.data.binary.lhs);
                                    int l_id = reg_id - 1;
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        opnum2 += std::to_string(reg_id);
                                    }
                                    else 
                                        opnum2 = "x0";
                                    
                                    // 执行sub指令
                                    res += "  sub t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum2;
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += "\n";

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
                                    int r_id = reg_id - 1;
                                    
                                    // 执行sub指令
                                    res += "  sub t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

                                    // 分配一个reg_id
                                    reg_id ++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case KOOPA_RBO_MUL:
                {
                    switch (kind.data.binary.lhs->kind.tag)
                    {
                        case KOOPA_RVT_INTEGER:
                        {
                            switch (kind.data.binary.rhs->kind.tag)
                            {
                                case KOOPA_RVT_INTEGER:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        if (!zero1) res += std::to_string(reg_id + 1);
                                        else res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        if (!zero1) opnum2 += std::to_string(reg_id + 1);
                                        else opnum2 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum2 = "x0";

                                    // 执行mul指令
                                    res += "  mul t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", ";
                                    res += opnum2;
                                    res += "\n";

                                    // 分配一个reg_id
                                    reg_id ++;
                                    break;
                                }
                                case KOOPA_RVT_BINARY:
                                {
                                    // 操作数准备
                                    if (kind.data.binary.lhs->kind.data.integer.value == 0) zero1 = true;
                                    if (!Is_Used_By(value, kind.data.binary.rhs)) res += Visit(kind.data.binary.rhs);
                                    int r_id = reg_id - 1;
                                    if (!zero1)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.lhs->kind.data.integer);
                                        res += "\n";
                                        opnum1 = "t";
                                        opnum1 += std::to_string(reg_id);
                                    }
                                    else
                                        opnum1 = "x0";

                                    // 执行mul指令
                                    res += "  mul t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum1;
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

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
                                    if (kind.data.binary.rhs->kind.data.integer.value == 0) zero2 = true;
                                    if (!Is_Used_By(value, kind.data.binary.lhs)) res += Visit(kind.data.binary.lhs);
                                    int l_id = reg_id - 1;
                                    if (!zero2)
                                    {
                                        res += "  li t";
                                        res += std::to_string(reg_id);
                                        res += ", ";
                                        res += Visit(kind.data.binary.rhs->kind.data.integer);
                                        res += "\n";
                                        opnum2 = "t";
                                        opnum2 += std::to_string(reg_id);
                                    }
                                    else 
                                        opnum2 = "x0";
                                    
                                    // 执行mul指令
                                    res += "  mul t";
                                    res += std::to_string(reg_id);
                                    res += ", ";
                                    res += opnum2;
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += "\n";

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
                                    
                                    // 执行mul指令
                                    res += "  mul t";
                                    res += std::to_string(reg_id);
                                    res += ", t";
                                    res += std::to_string(l_id);
                                    res += ", t";
                                    res += std::to_string(r_id);
                                    res += "\n";

                                    // 分配一个reg_id
                                    reg_id ++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case KOOPA_RBO_DIV:
                {

                }
                case KOOPA_RBO_MOD:
                {

                }
                case KOOPA_RBO_AND:
                {

                }
                case KOOPA_RBO_OR:
                {

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