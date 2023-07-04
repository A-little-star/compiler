#include "xc.hpp"
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <set>

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
    bool changed = false;
    basic_block_ptr b = NULL, b1 = NULL, b2 = NULL;
    std::set<value_ptr> newin;

    for (size_t i = 0; i < bbs->len; i ++ ) {
        basic_block_ptr bb = (basic_block_ptr)bbs->buffer[i];
        bb->ComputeDefAndLiveUse();
    }

    changed = true;
    while (changed) {
        changed = false;

        for (int i = bbs->len - 1; i >= 0; i -- ) {
            b = (basic_block_ptr)bbs->buffer[i];
            
            if (b->insts->len == 0) continue;
            value_ptr end = (value_ptr)b->insts->buffer[b->insts->len - 1];
            switch (end->kind.tag) {
                case IR_JUMP:
                    b1 = b->next[0];
                    b->liveout = b1->livein;
                    break;

                case IR_BRANCH:
                    b1 = b->next[0];
                    b2 = b->next[1];
                    b->liveout.clear();
                    std::set_union(b1->livein.begin(), b1->livein.end(), b2->livein.begin(), b2->livein.end(), std::inserter(b->liveout, b->liveout.begin()));
                    break;

                case IR_RETURN:
                    break;
                
                default:
                    assert(false);
            }

            std::set<value_ptr> ss;
            std::set_difference(b->liveout.begin(), b->liveout.end(), b->def.begin(), b->def.end(), std::inserter(ss, ss.begin()));
            newin.clear();
            std::set_union(ss.begin(), ss.end(), b->liveuse.begin(), b->liveuse.end(), std::inserter(newin, newin.begin()));
            if (!(newin == b->livein)) {
                changed = true;
                b->livein = newin;
            }
        }
    }
}

void basic_block::AnalyzeLiveness(void) {
    if (insts->len == 0)
        return;

    value_ptr inst = (value_ptr)insts->buffer[insts->len - 1];
    inst->liveout = liveout;
    if (inst->kind.tag == IR_BRANCH){
        inst->liveout.insert(inst->kind.data.branch.cond);
    }
    if (inst->kind.tag == IR_RETURN && inst->kind.data.ret.value != NULL) {
        inst->liveout.insert(inst->kind.data.ret.value);
    }

    for (int i = insts->len - 2; i >= 0; i -- ) {
        value_ptr inst = (value_ptr)insts->buffer[i];
        value_ptr inst_next = (value_ptr)insts->buffer[i + 1];
        inst->liveout = inst_next->liveout;
        switch (inst_next->kind.tag) {
            case IR_ALLOC:
                inst->liveout.erase(inst_next);
                break;
            case IR_LOAD:
                inst->liveout.erase(inst_next);
                inst->liveout.insert(inst_next->kind.data.load.src);
                break;
            case IR_STORE:
                if (inst_next->kind.data.store.value->kind.tag != IR_INTEGER)
                    inst->liveout.insert(inst_next->kind.data.store.value);
                inst->liveout.insert(inst_next->kind.data.store.dest);
                break;
            case IR_GET_PTR:
                inst->liveout.erase(inst_next);
                if (inst_next->kind.data.get_ptr.index->kind.tag != IR_INTEGER)
                    inst->liveout.insert(inst_next->kind.data.get_ptr.index);
                inst->liveout.insert(inst_next->kind.data.get_ptr.src);
                break;
            case IR_GET_ELEM_PTR:
                inst->liveout.erase(inst_next);
                if (inst_next->kind.data.get_elem_ptr.index->kind.tag != IR_INTEGER)
                    inst->liveout.insert(inst_next->kind.data.get_elem_ptr.index);
                inst->liveout.insert(inst_next->kind.data.get_elem_ptr.src);
                break;
            case IR_BINARY:
                inst->liveout.erase(inst_next);
                if (inst_next->kind.data.binary.lhs->kind.tag != IR_INTEGER)
                    inst->liveout.insert(inst_next->kind.data.binary.lhs);
                if (inst_next->kind.data.binary.rhs->kind.tag != IR_INTEGER)
                    inst->liveout.insert(inst_next->kind.data.binary.rhs);
                break;
            case IR_BRANCH:
                if (inst_next->kind.data.branch.cond->kind.tag != IR_INTEGER)
                    inst->liveout.insert(inst_next->kind.data.branch.cond);
                break;
            case IR_CALL:
                inst->liveout.erase(inst_next);
                for (size_t i = 0; i < inst_next->kind.data.call.args->len; i ++ ) {
                    value_ptr arg = (value_ptr)inst_next->kind.data.call.args->buffer[i];
                    if (arg->kind.tag != IR_INTEGER)
                        inst->liveout.insert(arg);
                }
                break;
            case IR_RETURN:
                if (inst_next->kind.data.ret.value->kind.tag != IR_INTEGER)
                    inst->liveout.insert(inst_next->kind.data.ret.value);
                break;
            case IR_JUMP:
                break;
            default: assert(false);
        }
    }
}

void program::AnalyzeLiveness(void) {
    for (size_t i = 0; i < funcs->len; i ++ ) {
        func_ptr func = (func_ptr)funcs->buffer[i];
        func->AnalyzeLiveness();
        for (size_t j = 0; j < func->bbs->len; j ++ ) {
            basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
            bb->AnalyzeLiveness();
        }
    }
}