#include <cstdio>
#include <iostream>
#include <assert.h>
#include "xc.hpp"

std::string irDS2Text(prog_ptr prog) {
    printf("function number : %d\n", prog->funcs->len);
    for (int i = 0; i < 1 && i < prog->funcs->len; i ++ ) {
        assert(prog->funcs->kind == RISK_FUNCTION);
        func_ptr func = (func_ptr)prog->funcs->buffer[i];
        for (int j = 0; j < 1 && j < func->bbs->len; j ++ ) {
            assert(func->bbs->kind == RISK_BASIC_BLOCK);
            basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
            for (int k = 0; k < 1 && k < bb->insts->len; k ++ ) {
                assert(bb->insts->kind == RISK_INSTRUCTION);
                value_ptr value = (value_ptr)bb->insts->buffer[i];
                assert(value->kind.tag == IR_RETURN);
                value_ptr ret_value = value->kind.data.ret.value;
                assert(ret_value->kind.tag == IR_INTEGER);
                int int_val = ret_value->kind.data.integer.value;
                printf("ret 0\n");
            }
        }
    }
    return "";
}