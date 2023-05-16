#include <cstring>
#include <cassert>
#include <iostream>
#include "koopa.h"
#include "vkoopa.h"

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

// 访问指令、全局变量
std::string Visit(const koopa_raw_value_t &value)
{
    std::string res;
    const auto &kind = value->kind;
    switch (kind.tag)
    {
        case KOOPA_RVT_RETURN:
            res += Visit(kind.data.ret);
            res += "  ret";
            break;
        case KOOPA_RVT_INTEGER:
            res += Visit(kind.data.integer);
            break;
        default:
            // 其他类型暂时不考虑
            assert(false);
    }
    return res;
}

std::string Visit(const koopa_raw_return_t &ret)
{
    std::string res = "  li a0, ";
    res += Visit(ret.value);
    res += '\n';
    return res;
}

std::string Visit(const koopa_raw_integer_t &integer)
{
    return std::to_string(integer.value);
}