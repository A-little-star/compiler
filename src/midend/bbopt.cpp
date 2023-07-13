#include "bbopt.hpp"

static void dfs(basic_block_ptr bb, std::unordered_set<basic_block_ptr> &vis) {
    if (!vis.count(bb)) {
        vis.insert(bb);
        for (basic_block_ptr x : bb->next) {
            if (x) dfs(x, vis);
        }
    }
}

void bbopt(func_ptr func) {
    
    std::unordered_set<basic_block_ptr> vis;
    vis.clear();
    dfs((basic_block_ptr)func->bbs->buffer[0], vis);
    for (int i = 0; i < func->bbs->len; i ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
        if (bb->insts->len == 0) {
            func->DeleteBB(bb);
            continue;
        }
        if (!vis.count(bb)) {
            for (basic_block_ptr s : bb->next) {
                if (s && vis.count(s)) {
                    u32 idx = std::find(s->pred.begin(), s->pred.end(), bb) - s->pred.begin();
                    s->pred.erase(s->pred.begin() + idx);
                }
            }
        }
    }
    
    for (int i = 0; i < func->bbs->len; i ++ ) {
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[i];
        if (!vis.count(bb)) {
            func->DeleteBB(bb);
            delete bb;
            i --;
        }
    }
}