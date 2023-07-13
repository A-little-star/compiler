#include "mem2reg.hpp"

void insertBBParam(basic_block_ptr bb, value_ptr param) {
    bb->params->len ++;
    bb->params->buffer.push_back(param);
}

void replaceValue(func_ptr func, value_ptr value, value_ptr dest) {
    for (int j = 0; j < func->bbs->len; j ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[j];
        for (int k = 0; k < bb->insts->len; k ++ ) {
            value_ptr inst = (value_ptr)bb->insts->buffer[k];
            switch (inst->kind.tag) {
                case IR_ALLOC:
                    break;
                case IR_BINARY:
                    if (inst->kind.data.binary.lhs == value) inst->kind.data.binary.lhs = dest;
                    if (inst->kind.data.binary.rhs == value) inst->kind.data.binary.rhs = dest;
                    break;
                case IR_BRANCH:
                    if (inst->kind.data.branch.cond == value) inst->kind.data.branch.cond = dest;
                    break;
                case IR_CALL:
                    for (int i = 0; i < inst->kind.data.call.args->len; i ++ ) {
                        value_ptr arg = (value_ptr)inst->kind.data.call.args->buffer[i];
                        if (arg == value) inst->kind.data.call.args->buffer[i] = (void*)dest;
                    }
                    break;
                case IR_GET_ELEM_PTR:
                    if (inst->kind.data.get_elem_ptr.index == value) inst->kind.data.get_elem_ptr.index = dest;
                    if (inst->kind.data.get_elem_ptr.src == value) inst->kind.data.get_elem_ptr.src = dest;
                    break;
                case IR_GET_PTR:
                    if (inst->kind.data.get_ptr.index == value) inst->kind.data.get_ptr.index = dest;
                    if (inst->kind.data.get_ptr.src == value) inst->kind.data.get_ptr.src = dest;
                    break;
                case IR_JUMP:
                    break;
                case IR_LOAD:
                    if (inst->kind.data.load.src == value) inst->kind.data.load.src = dest;
                    break;
                case IR_RETURN:
                    if (inst->kind.data.ret.value == value) inst->kind.data.ret.value = dest;
                    break;
                case IR_STORE:
                    if (inst->kind.data.store.dest == value) inst->kind.data.store.dest = dest;
                    if (inst->kind.data.store.value == value) inst->kind.data.store.value = dest;
                    break;
                default: assert(false);
            }
        }
    }
}

void mem2reg(func_ptr func) {
    BuildDomTree(func);
    std::unordered_map<value_ptr, u32> alloc_ids;
    std::vector<value_ptr> allocs;
    for (int i = 0; i < func->bbs->len; i ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
        if (bb->insts->len == 0) continue;
        for (int j = 0; j < bb->insts->len; j ++ ) {
            value_ptr inst = (value_ptr)bb->insts->buffer[j];
            if (inst->kind.tag == IR_ALLOC) {
                alloc_ids.insert({inst, (u32)alloc_ids.size()});
                allocs.push_back(inst);
            }
        }
    }
    std::vector<std::vector<basic_block_ptr>> alloc_defs(alloc_ids.size());
    for (int i = 0; i < func->bbs->len; i ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
        if (bb->insts->len == 0) continue;
        for (int j = 0; j < bb->insts->len; j ++ ) {
            value_ptr inst = (value_ptr)bb->insts->buffer[j];
            if (inst->kind.tag == IR_STORE) {
                auto it = alloc_ids.find(inst->kind.data.store.dest);
                if (it != alloc_ids.end()) {
                    alloc_defs[it->second].push_back(bb);
                }
            }
        }
    }
    ComputeDF(func);
    // mem2reg算法阶段1：放置phi结点（基本块参数）
    std::vector<basic_block_ptr> worklist;
    std::unordered_map<value_ptr, u32> phis;
    std::unordered_set<basic_block_ptr> vis;
    for (u32 id = 0; id < allocs.size(); id ++ ) {
        vis.clear();
        for (basic_block_ptr bb : alloc_defs[id]) {
            worklist.push_back(bb);
        }
        while (!worklist.empty()) {
            basic_block_ptr x = worklist.back();
            worklist.pop_back();
            for (basic_block_ptr y : x->df) {
                if (!vis.count(y)) {
                    vis.insert(y);
                    value_ptr param = new value;
                    phis.insert({param, id});
                    param->ty = *allocs[id]->ty.data.pointer.base;
                    param->kind.tag = IR_BLOCK_ARG;
                    param->kind.data.block_arg.index = y->params->len + 1;
                    param->name = allocs[id]->name + "_" + std::to_string(phis[param]);
                    insertBBParam(y, param);
                    worklist.push_back(y);
                }
            }
        }
    }
    // mem2reg算法阶段2：变量重命名，即删除load，把load结果的引用换成对寄存器的引用，把store改成寄存器赋值
    std::vector<std::pair<basic_block_ptr, std::vector<value_ptr>>> worklist2{
        {(basic_block_ptr)func->bbs->buffer[0], std::vector<value_ptr>(alloc_ids.size())}
    };
    vis.clear();
    while (!worklist2.empty()) {
        basic_block_ptr bb = worklist2.back().first;
        std::vector<value_ptr> values = worklist2.back().second;
        worklist2.pop_back();
        
        if (!vis.count(bb)) {
            vis.insert(bb);
            for (int i = 0; i < bb->params->len; i ++ ) {
                value_ptr param = (value_ptr)bb->params->buffer[i];
                auto it = phis.find(param);
                if (it != phis.end()) {
                    values[it->second] = param;
                }
                else {
                    assert(false);
                }
            }
            for (int i = 0; i < bb->insts->len; i ++ ) {
                value_ptr inst = (value_ptr)bb->insts->buffer[i];
                if (auto it = alloc_ids.find(inst); it != alloc_ids.end()) {
                    bb->DeleteInst(inst);
                    delete inst;
                    i --;
                } else if (inst->kind.tag == IR_LOAD) {
                    auto it = alloc_ids.find(inst->kind.data.load.src);
                    if (it != alloc_ids.end()) {
                        replaceValue(func, inst, values[it->second]);
                        bb->DeleteInst(inst);
                        inst->kind.data.load.src = nullptr;
                        delete inst;
                        i --;
                    }
                    else {
                        assert(false);
                    }
                } else if (inst->kind.tag == IR_STORE) {
                    auto it = alloc_ids.find(inst->kind.data.store.dest);
                    if (it != alloc_ids.end()) {
                        values[it->second] = inst->kind.data.store.value;
                        bb->DeleteInst(inst);
                        inst->kind.data.store.dest = nullptr;
                        delete inst;
                        i --;
                    }
                    else {
                        assert(false);
                    }
                }
            }
            for (basic_block_ptr x : bb->next) {
                if (x) {
                    worklist2.emplace_back(x, values);
                    for (int i = 0; i < x->params->len; i ++ ) {
                        value_ptr param = (value_ptr)x->params->buffer[i];
                        auto it = phis.find(param);
                        if (it != phis.end()) {
                            // for (auto y : x->pred) {
                                value_ptr end_value = (value_ptr)bb->insts->buffer[bb->insts->len - 1];
                                switch (end_value->kind.tag) {
                                    case IR_JUMP:
                                        assert(end_value->kind.data.jump.args != NULL);
                                        end_value->kind.data.jump.args->len ++;
                                        end_value->kind.data.jump.args->buffer.push_back(values[it->second]);
                                        break;
                                    case IR_BRANCH:
                                        assert(end_value->kind.data.branch.true_args != NULL);
                                        assert(end_value->kind.data.branch.false_args != NULL);
                                        if (end_value->kind.data.branch.true_bb == x) {
                                            end_value->kind.data.branch.true_args->len ++;
                                            end_value->kind.data.branch.true_args->buffer.push_back(values[it->second]);
                                        }
                                        else if (end_value->kind.data.branch.false_bb == x) {
                                            end_value->kind.data.branch.false_args->len ++;
                                            end_value->kind.data.branch.false_args->buffer.push_back(values[it->second]);
                                        }
                                        break;
                                    case IR_RETURN:
                                        break;
                                    default: assert(false);
                                }
                            // }
                        }
                    }
                }
            }
        }
    }
}