#include "koopa.h"
#include <cstring>
#include <string>
#include <iostream>
#include <cassert>

std::string Visit(const koopa_raw_program_t &program);
std::string Visit(const koopa_raw_slice_t &slice);
std::string Visit(const koopa_raw_function_t &func);
std::string Visit(const koopa_raw_basic_block_t &bb);
std::string Visit(const koopa_raw_value_t &value);
std::string Visit(const koopa_raw_return_t &ret);
std::string Visit(const koopa_raw_integer_t &integer);
bool Is_Used_By(const koopa_raw_value_t &v1, const koopa_raw_value_t &v2);