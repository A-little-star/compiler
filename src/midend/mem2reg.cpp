#include "mem2reg.hpp"

void insertBBParam(basic_block_ptr bb, value_ptr param) {
    bb->params->len ++;
    bb->params->buffer.push_back(param);
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

}