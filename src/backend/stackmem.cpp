#include "../midend/xc.hpp"
#include <unordered_map>
#include <algorithm>
#include <assert.h>

std::unordered_map<value_ptr, int> off_map;
std::unordered_map<value_ptr, int> arg_off_map;
std::unordered_map<func_ptr, int> has_call;

int SizeOfType(type_kind ty) {
    switch (ty.tag) {
        case KOOPA_TYPE_ARRAY: return SizeOfType(*ty.data.array.base) * ty.data.array.len;
        case KOOPA_TYPE_POINTER: return 4;
        case KOOPA_TYPE_INT32: return 4;
        default: assert(false);
    }
}

int CalStackMem(const func_ptr func) {
    int s = 0, r = 0, a = 0;
    for (size_t j = 0; j < func->bbs->len; j ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[j];
        for (size_t k = 0; k < bb->insts->len; k ++ ) {
            value_ptr inst = (value_ptr)bb->insts->buffer[k];
            switch (inst->kind.tag) {
                case IR_ALLOC: {
                    s += 4;
                    type_kind type = *inst->ty.data.pointer.base;
                    s += SizeOfType(type);
                    break;
                }
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
    for (size_t j = 8; j < func->params->len; j ++ ) {
        value_ptr param = (value_ptr)func->params->buffer[j];
        param->offset = s + r + a + (j - 8) * 4;
    }
    for (size_t j = 0; j < func->bbs->len; j ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[j];
        for (size_t k = 0; k < bb->insts->len; k ++ ) {
            value_ptr inst = (value_ptr)bb->insts->buffer[k];
            switch (inst->kind.tag) {
                case IR_CALL: {
                    if (inst->ty.tag == KOOPA_TYPE_INT32) {
                        off_map[inst] = s + a - num - 4;
                        inst->offset = off_map[inst];
                        num += 4;
                    }
                    for (size_t i = 8; i < inst->kind.data.call.args->len; i ++ ) {
                        value_ptr arg = (value_ptr)inst->kind.data.call.args->buffer[i];
                        arg_off_map[arg] = (i - 8) * 4;
                        arg->arg_offset = arg_off_map[arg];
                    }
                    break;
                }
                case IR_ALLOC: {
                    off_map[inst] = s + a - num - 4;
                    inst->offset = off_map[inst];
                    num += 4;
                    num += SizeOfType(*inst->ty.data.pointer.base);
                    break;
                }
                case IR_BINARY:
                case IR_LOAD: {
                    off_map[inst] = s + a - num - 4;
                    inst->offset = off_map[inst];
                    num += 4;
                    break;
                }
                case IR_STORE:
                case IR_BRANCH:
                case IR_JUMP:
                case IR_RETURN: break;
                default: assert(false);
            }
        }
    }
    return s + r + a;
}