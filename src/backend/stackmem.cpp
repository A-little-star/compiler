#include "../midend/xc.hpp"
#include <unordered_map>
#include <algorithm>

std::unordered_map<value_ptr, int> off_map;
std::unordered_map<value_ptr, int> arg_off_map;
std::unordered_map<func_ptr, int> has_call;

int CalStackMem(const func_ptr func) {
    int s = 0, r = 0, a = 0;
    for (size_t j = 0; j < func->bbs->len; j ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[j];
        for (size_t k = 0; k < bb->insts->len; k ++ ) {
            value_ptr inst = (value_ptr)bb->insts->buffer[k];
            switch (inst->kind.tag) {
                case IR_ALLOC: s += 4; break;
                case IR_BINARY: s += 4; break;
                case IR_LOAD: s += 4; break;
                case IR_CALL: {
                    has_call[func] = 1;
                    r = 4;

                    if (inst->ty.tag == KOOPA_TYPE_INT32) s += 4;
                    else if (inst->ty.tag == KOOPA_TYPE_UNIT) s += 0;

                    if (inst->kind.data.call.args->len > 8) {
                        int len = 4 * (inst->kind.data.call.args->len - 8);
                        a = std::max(a, len);
                    }
                    break;
                }
                default: break;
            }
        }
    }

    int num = 0;
    for (size_t j = 0; j < func->bbs->len; j ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[j];
        for (size_t k = 0; k < bb->insts->len; k ++ ) {
            value_ptr inst = (value_ptr)bb->insts->buffer[k];
            switch (inst->kind.tag) {
                case IR_CALL: {
                    if (inst->ty.tag == KOOPA_TYPE_INT32) {
                        off_map[inst] = s + a - 4 * num - 4;
                        num ++;
                    }
                    for (size_t i = 8; i < inst->kind.data.call.args->len; i ++ ) {
                        value_ptr arg = (value_ptr)inst->kind.data.call.args->buffer[i];
                        arg_off_map[arg] = (i - 8) * 4;
                    }
                    break;
                }
                case IR_ALLOC:
                case IR_BINARY:
                case IR_LOAD: {
                    off_map[inst] = s + a - 4 * num - 4;
                    num ++;
                    break;
                }
                default: break;
            }
        }
    }
    return s + r + a;
}