#include "xc.hpp"
#include "ssa.hpp"
#include <unordered_set>
#include <algorithm>

static void dfs(basic_block_ptr bb, u32 dom_level) {
    bb->dom_level = dom_level;
    for (basic_block_ptr ch : bb->doms) {
        dfs(ch, dom_level + 1);
    }
}

void BuildDomTree(func_ptr func) {
    basic_block_ptr entry = (basic_block_ptr)func->bbs->buffer[0];
    // 计算dom_by
    entry->dom_by = {entry};
    std::unordered_set<basic_block_ptr> all;  // 全部基本块，除entry以外的dom的初值
    for (int i = 0; i < func->bbs->len; i ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
        if (bb->insts->len == 0) continue;
        all.insert(bb);
        bb->doms.clear();
    }
    for (int i = 1; i < func->bbs->len; i ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
        if (bb->insts->len == 0) continue;
        bb->dom_by = all;
    }
    while (true) {
        bool changed = false;
        for (int i = 1; i < func->bbs->len; i ++ ) {
            basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
            if (bb->insts->len == 0) continue;
            for (auto it = bb->dom_by.begin(); it != bb->dom_by.end(); ) {
                basic_block_ptr x = *it;
                // 如果bb的任何一个pred的dom不包含x，那么bb的dom也不应该包含x
                if (x != bb && std::any_of(bb->pred.begin(), bb->pred.end(), [x](basic_block_ptr p) { return p->dom_by.find(x) == p->dom_by.end(); })) {
                    changed = true;
                    it = bb->dom_by.erase(it);
                } else {
                    ++ it;
                }
            }
        }
        if (!changed) { break; }
    }
    entry->idom = nullptr;
    for (int i = 1; i < func->bbs->len; i ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
        if (bb->insts->len == 0) continue;
        for (basic_block_ptr d : bb->dom_by) {
            if (d != bb && std::all_of(bb->dom_by.begin(), bb->dom_by.end(), [d, bb](basic_block_ptr x) {
                return x == bb || x == d || x->dom_by.find(d) == x->dom_by.end();
            })) {
                bb->idom = d;
                d->doms.push_back(bb);
                break;
            }
        }
    }
    dfs(entry, 0);
}

void toSSA(prog_ptr prog) {
    for (int i = 0; i < prog->funcs->len; i ++ ) {
        func_ptr func = (func_ptr)prog->funcs->buffer[i];
        BuildDomTree(func);
    }
}