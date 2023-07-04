#include "xc.hpp"
#include <assert.h>

void basic_block::UpdateLU(value_ptr v) {
    if (v != NULL && def.count(v) == 0)
        liveuse.insert(v);
}

void basic_block::UpdateDEF(value_ptr v) {
    if (v != NULL)
        def.insert(v);
}

void basic_block::ComputeDefAndLiveUse(void) {
    for (size_t i = 0; i < insts->len; i ++ ) {
        value_ptr var = (value_ptr)insts->buffer[i];
        switch (var->kind.tag) {
            case IR_ALLOC:
                UpdateDEF(var);
                break;
            case IR_LOAD:
                UpdateLU(var->kind.data.load.src);
                UpdateDEF(var);
                break;
            case IR_STORE:
                if (var->kind.data.store.value->kind.tag != IR_INTEGER) 
                    UpdateLU(var->kind.data.store.value);
                UpdateLU(var->kind.data.store.dest);
                break;
            case IR_GET_PTR:
                if (var->kind.data.get_ptr.index->kind.tag != IR_INTEGER)
                    UpdateLU(var->kind.data.get_ptr.index);
                UpdateLU(var->kind.data.get_ptr.src);
                UpdateDEF(var);
                break;
            case IR_GET_ELEM_PTR:
                if (var->kind.data.get_elem_ptr.index->kind.tag != IR_INTEGER) 
                    UpdateLU(var->kind.data.get_elem_ptr.index);
                UpdateLU(var->kind.data.get_elem_ptr.src);
                UpdateDEF(var);
                break;
            case IR_BINARY:
                if (var->kind.data.binary.lhs->kind.tag != IR_INTEGER) 
                    UpdateLU(var->kind.data.binary.lhs);
                if (var->kind.data.binary.rhs->kind.tag != IR_INTEGER)
                    UpdateLU(var->kind.data.binary.rhs);
                UpdateDEF(var);
                break;
            case IR_BRANCH:
                if (var->kind.data.branch.cond->kind.tag != IR_INTEGER)
                    UpdateLU(var->kind.data.branch.cond);
                break;
            case IR_CALL:
                for (size_t i = 0; i < var->kind.data.call.args->len; i ++ ) {
                    value_ptr arg = (value_ptr)var->kind.data.call.args->buffer[i];
                    if (arg->kind.tag != IR_INTEGER)
                        UpdateLU(arg);
                }
                UpdateDEF(var);
                break;
            case IR_RETURN:
                if (var->kind.data.ret.value->kind.tag != IR_INTEGER)
                    UpdateLU(var->kind.data.ret.value);
                break;
            case IR_JUMP:
                break;
            default: assert(false);
        }
    }
}

void function::AnalyzeLiveness(void) {
    for (size_t i = 0; i < bbs->len; i ++ ) {
        basic_block_ptr bb = (basic_block_ptr)bbs->buffer[i];
        bb->ComputeDefAndLiveUse();
    }
}

void program::AnalyzeLiveness(void) {
    for (size_t i = 0; i < funcs->len; i ++ ) {
        func_ptr func = (func_ptr)funcs->buffer[i];
        func->AnalyzeLiveness();
    }
}