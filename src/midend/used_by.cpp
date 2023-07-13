#include "used_by.hpp"

void ComputeUsedBy(prog_ptr prog) {
    
    for (int i = 0; i < prog->funcs->len; i ++ ) {
        func_ptr func = (func_ptr)prog->funcs->buffer[i];
        for (int j = 0; j < func->bbs->len; j ++ ) {
            basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[j];
            for (int k = 0; k < bb->insts->len; k ++ ) {
                value_ptr inst = (value_ptr)bb->insts->buffer[k];
                switch (inst->kind.tag) {
                    case IR_ALLOC:
                        break;
                    case IR_BINARY:
                        if (inst->kind.data.binary.lhs->kind.tag != IR_INTEGER) {
                            inst->kind.data.binary.lhs->used_by->len ++;
                            inst->kind.data.binary.lhs->used_by->buffer.push_back(inst);
                        }
                        if (inst->kind.data.binary.rhs->kind.tag != IR_INTEGER) {
                            inst->kind.data.binary.rhs->used_by->len ++;
                            inst->kind.data.binary.rhs->used_by->buffer.push_back(inst);
                        }
                        break;
                    case IR_BRANCH:
                        if (inst->kind.data.branch.cond->kind.tag != IR_INTEGER) {
                            inst->kind.data.branch.cond->used_by->len ++;
                            inst->kind.data.branch.cond->used_by->buffer.push_back(inst);
                        }
                        break;
                    case IR_CALL:
                        for (int i = 0; i < inst->kind.data.call.args->len; i ++ ) {
                            value_ptr arg = (value_ptr)inst->kind.data.call.args->buffer[i];
                            if (arg->kind.tag != IR_INTEGER) {
                                arg->used_by->len ++;
                                arg->used_by->buffer.push_back(arg);
                            }
                        }
                        break;
                    case IR_GET_ELEM_PTR:
                        if (inst->kind.data.get_elem_ptr.index->kind.tag != IR_INTEGER) {
                            inst->kind.data.get_elem_ptr.index->used_by->len ++;
                            inst->kind.data.get_elem_ptr.index->used_by->buffer.push_back(inst);
                        }
                        if (inst->kind.data.get_elem_ptr.src->kind.tag != IR_INTEGER) {
                            inst->kind.data.get_elem_ptr.src->used_by->len ++;
                            inst->kind.data.get_elem_ptr.src->used_by->buffer.push_back(inst);
                        }
                        break;
                    case IR_GET_PTR:
                        if (inst->kind.data.get_ptr.index->kind.tag != IR_INTEGER) {
                            inst->kind.data.get_ptr.index->used_by->len ++;
                            inst->kind.data.get_ptr.index->used_by->buffer.push_back(inst);
                        }
                        if (inst->kind.data.get_ptr.src->kind.tag != IR_INTEGER) {
                            inst->kind.data.get_ptr.src->used_by->len ++;
                            inst->kind.data.get_ptr.src->used_by->buffer.push_back(inst);
                        }
                        break;
                    case IR_JUMP:
                        break;
                    case IR_LOAD:
                        if (inst->kind.data.load.src->kind.tag != IR_INTEGER) {
                            inst->kind.data.load.src->used_by->len ++;
                            inst->kind.data.load.src->used_by->buffer.push_back(inst);
                        }
                        break;
                    case IR_RETURN:
                        break;
                    case IR_STORE:
                        inst->kind.data.store.dest->used_by->len ++;
                        inst->kind.data.store.dest->used_by->buffer.push_back(inst);
                        if (inst->kind.data.store.value->kind.tag != IR_INTEGER) {
                            inst->kind.data.store.value->used_by->len ++;
                            inst->kind.data.store.value->used_by->buffer.push_back(inst);
                        }
                        break;
                    default: assert(false);
                }
            }
        }
    }
}