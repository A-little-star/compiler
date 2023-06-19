#include "../midend/xc.hpp"
#include <unordered_map>

std::unordered_map<value_ptr, int> off_map;

int CalStackMem(const func_ptr func) {
    int offset = 0;
    for (size_t j = 0; j < func->bbs->len; j ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[j];
        for (size_t k = 0; k < bb->insts->len; k ++ ) {
            value_ptr inst = (value_ptr)bb->insts->buffer[k];
            off_map[inst] = offset;
            switch (inst->kind.tag) {
                case IR_ALLOC: offset += 4; break;
                case IR_BINARY: offset += 4; break;
                case IR_LOAD: offset += 4; break;
                default: break;
            }
        }
    }
    return offset;
}