#include <memory>
#include <cstring>
#include <assert.h>
#include "xc.hpp"
#include "../frontend/AST.hpp"
#include "SymTable.hpp"
#include "loop.hpp"
// #include "visitor.hpp"

class Visitor;
static int bb_id;
std::unordered_map<StmtAST*, basic_block_ptr> end_map;
std::unordered_map<std::string, int> rep_map;
std::unordered_map<std::string, func_ptr> func_map;
std::unordered_map<value_ptr, value_ptr> param2value;

class GenIR : public Visitor {
    void *visit(CompUnitAST *CompUnit) {
        // 新建一个program.
        prog_ptr prog = tr->NewProgram();
        tr->prog_cur = prog;
        tr->is_global = false;
        // 创建语句块树的根节点
        BlockTreeNode *root_bt = new BlockTreeNode;
        root_bt->father_block = root_bt;
        root_bt->block = NULL;
        
        bt.root = root_bt;
        bt.current = root_bt;

        // 创建循环树的根节点
        LoopTreeNode *root_lt = new LoopTreeNode;
        root_lt->father_loop = root_lt;
        root_lt->loop_body = NULL;
        root_lt->loop_entry = NULL;
        root_lt->loop_end = NULL;
        lt.root = root_lt;
        lt.current = root_lt;

        // 声明SysY库中的函数
        tr->AddLibFunc();
        // 递归调用program下的funcdef
        CompUnit->compitems->accept(this);
        
        // 回收内存
        delete root_bt;
        root_bt = NULL;
        delete root_lt;
        root_lt = NULL;
        return prog;
    }
    void *visit(CompItemsAST *CompItems) {
        for (size_t i = 0; i < CompItems->compitems.size(); i ++ ) {
            CompItems->compitems[i]->accept(this);
        }
        return NULL;
    }
    void *visit(CompItemAST *CompItem) {
        if (CompItem->type == CompItemAST::FUNC) {
            func_ptr func = (func_ptr)CompItem->funcdef->accept(this);
            tr->AddFunc(func);
        }
        else if (CompItem->type == CompItemAST::DECL) {
            tr->is_global = true;
            CompItem->decl->accept(this);
            tr->is_global = false;
        }
        return NULL;
    }
    void *visit(FuncDefAST *FuncDef) {
        // 创建一个新的函数类型，并为其成员变量分配空间
        func_ptr func = tr->NewFunction();
        // 进入funcdef之后，需要开启一个新的作用域
        BlockTreeNode *btn = new BlockTreeNode;
        btn->symtable = bt.current->symtable;
        btn->father_block = bt.current;
        bt.current = btn;

        tr->func_cur = func;
        
        // 函数名
        func->name = "@" + FuncDef->ident;
        // 递归处理函数的返回值类型
        if (FuncDef->func_type == "int") func->ty.tag = KOOPA_TYPE_INT32;
        else if (FuncDef->func_type == "void") func->ty.tag = KOOPA_TYPE_UNIT;
        else assert(false);
        // type_kind *ret_type = (type_kind*)FuncDef->func_type->accept(this);
        // func->ty = *ret_type;
        // 如果函数有形参列表，那么递归处理函数的形参列表
        if (FuncDef->type == FuncDefAST::HAS_PARAMS) {
            func->params = (slice_ptr)FuncDef->funcfparams->accept(this);
        }
        else {
            slice_ptr params = new slice;
            params->len = 0;
            func->params = params;
        }
        // 创建一个新的Basic block，并为其成员变量分配空间；
        basic_block_ptr bb = tr->NewBasicBlock();
        bb->name = "%entry";
        // helper 指针指向当前所在的基本块
        tr->bb_cur = bb;

        // 将该基本块添加到所在的函数中
        tr->AddBasicBlock(bb);

        // 更新func_map
        func_map[FuncDef->ident] = func;
        
        // 递归处理block语句块
        FuncDef->block->accept(this);
        // 处理完语句块之后，如果是void返回值函数，添加一条ret指令
        if (func->ty.tag == KOOPA_TYPE_UNIT) {
            value_ptr ret = tr->NewValue();
            ret->kind.tag = IR_RETURN;
            ret->kind.data.ret.value = NULL;
            tr->AddValue(ret);
        }
        // 更新func_map
        // func_map[FuncDef->ident] = func;
        // 释放空间
        bt.current = btn->father_block;
        btn->symtable.clear();
        delete btn;
        btn = NULL;
        return func;
    }
    void *visit(FuncFParamsAST *FuncFParams) {
        slice_ptr fparams = new slice;
        fparams->len = 0;
        for (size_t i = 0; i < FuncFParams->funcfparams.size(); i ++ ) {
            tr->fp_index_cur = i + 1;
            value_ptr fparam = (value_ptr)FuncFParams->funcfparams[i]->accept(this);
            tr->InsertParam(fparam, fparams);
        }
        return fparams;
    }
    void *visit(FuncFParamAST *FuncFParam) {
        value_ptr fparam = tr->NewValue();
        if (FuncFParam->btype == "int") {
            if (FuncFParam->type == FuncFParamAST::INT) {
                fparam->ty.tag = KOOPA_TYPE_INT32;
            }
            else if (FuncFParam->type == FuncFParamAST::ARRAY) {
                fparam->ty.tag = KOOPA_TYPE_POINTER;
                fparam->ty.data.pointer.base = new type_kind;
                std::vector<int> length = *(std::vector<int>*)FuncFParam->constexps->accept(this);
                type_kind *type = fparam->ty.data.pointer.base;
                // printf("length size: %zu\n", length.size());
                for (size_t i = 0; i < length.size(); i ++ ) {
                    type->tag = KOOPA_TYPE_ARRAY;
                    type->data.array.len = length[i];
                    type->data.array.base = new type_kind;
                    type = type->data.array.base;
                }
                type->tag = KOOPA_TYPE_INT32;
            }
        }
        else {
            printf("There is an exception in the visit of FuncFParamAST!\n");
            assert(false);
        }
        fparam->name = "@" + FuncFParam->ident;
        fparam->kind.tag = IR_FUNC_ARG;
        fparam->kind.data.func_arg.index = tr->fp_index_cur;
        
        return fparam;
    }

    void *visit(FuncTypeAST *FuncType) {
        // 新建一个函数返回值类型的指针
        type_kind *ret_type = new type_kind;
        if (FuncType->type == "int")
            ret_type->tag = KOOPA_TYPE_INT32;
        else if (FuncType->type == "void")
            ret_type->tag = KOOPA_TYPE_UNIT;
        else {
            printf("There is an exception in visit of FuncType!\n");
            assert(false);
        }
        return ret_type;
    }
    void *visit(BlockAST *Block) {
        if (Block->blockitems == NULL) {
            return NULL;
        }
        // 创建一个新的BlockTreeNode
        BlockTreeNode *block = new BlockTreeNode;
        block->father_block = bt.root;
        if (bt.current != bt.root) {
            block->symtable = bt.current->symtable;
            block->father_block = bt.current;
        }
        bt.current = block;
        // 访问语句块下的指令，指令将在处理过程中逐渐push到bb的buffer中
        Block->blockitems->accept(this);
        // 该语句块处理完毕，回归到父语句块
        bt.current = bt.current->father_block;
        // 该语句块已经处理完毕，回收内存
        block->symtable.clear();
        delete block;
        block = NULL;
        return NULL;
    }
    void *visit(BlockItemsAST *BlockItems) {
        for (size_t i = 0; i < BlockItems->blockitems.size(); i ++ ) {
            BlockItems->blockitems[i]->accept(this);
        }
        return NULL;
    }
    void *visit(BlockItemAST *BlockItem) {
        if (BlockItem->type == BlockItemAST::DECL) {
            BlockItem->decl->accept(this);
        }
        else if (BlockItem->type == BlockItemAST::STMT) {
            BlockItem->stmt->accept(this);
        }
        else {
            printf("There is a exception in visit of BlockItem!\n");
            assert(false);
        }
        return NULL;
    }
    void *visit(DeclAST *Decl) {
        
        if (Decl->type == DeclAST::CON_DECL) Decl->constdecl->accept(this);
        else if (Decl->type == DeclAST::VAR_DECL) Decl->vardecl->accept(this);
        return NULL;
    }
    void *visit(ConstDeclAST *ConstDecl) {
        if (ConstDecl->btype == "int") tr->ty_cur.tag = KOOPA_TYPE_INT32;
        else {
            printf("There is an exception in visit of ConstDelAST!\n");
            assert(false);
        }
        ConstDecl->constdefs->accept(this);
        return NULL;
    }
    void *visit(ConstDefsAST *ConstDefs) {
        for (size_t i = 0; i < ConstDefs->constdefs.size(); i ++ ) {
            ConstDefs->constdefs[i]->accept(this);
        }
        return NULL;
    }
    void *visit(ConstDefAST *ConstDef) {
        if (ConstDef->d_type == ConstDefAST::VALUE) {
            int val = ConstDef->constinitval->get_value();
            if (tr->ty_cur.tag == KOOPA_TYPE_INT32) {
                bt.current->symtable[ConstDef->ident].ty.tag = I32;
                bt.current->symtable[ConstDef->ident].value = val;
                bt.current->symtable[ConstDef->ident].kind = CON;
                bt.current->symtable[ConstDef->ident].has_value = true;
                rep_map[ConstDef->ident] += 1;
            }
            else {
                printf("There is an exception in visit of ConstDef!\n");
                assert(false);
            }
        }
        else if (ConstDef->d_type == ConstDefAST::ARRAY) {
            bt.current->symtable[ConstDef->ident].has_value = false;
            bt.current->symtable[ConstDef->ident].kind = CON;
            
            rep_map[ConstDef->ident] ++;

            std::vector<int> length = *(std::vector<int>*)ConstDef->constexps->accept(this);
            tr->arr_len = length[0];

            type_t *type_p = &bt.current->symtable[ConstDef->ident].ty;
            
            for (size_t i = 0; i < length.size(); i ++ ) {
                type_p->tag = ARRAY;
                type_p->data = new type_t;
                type_p = type_p->data;
            }
            type_p->tag = POINTER;
            type_p->data = new type_t;
            type_p->data->tag = I32;
            
            if (!tr->is_global) {
                value_ptr val = tr->NewValue();
                val->ty.tag = KOOPA_TYPE_POINTER;
                val->ty.data.pointer.base = new type_kind;
                tr->arr.clear();
                // val->ty.tag = KOOPA_TYPE_ARRAY;
                type_kind *type = val->ty.data.pointer.base;
                tr->arr.push_back(type);
                for (size_t i = 0; i < length.size(); i ++ ) {
                    type->tag = KOOPA_TYPE_ARRAY;
                    type->data.array.len = length[i];
                    type->data.array.base = new type_kind;
                    type = type->data.array.base;
                    tr->arr.push_back(type);
                }
                type->tag = KOOPA_TYPE_POINTER;
                type->data.pointer.base = new type_kind;
                type->data.pointer.base->tag = KOOPA_TYPE_INT32;
                
                val->name = "@" + ConstDef->ident + '_' + std::to_string(rep_map[ConstDef->ident]);
                val->kind.tag = IR_ALLOC;
                tr->AddValue(val);
                tr->arr_cur = val;
                memset(tr->arr_index_cur, 0, sizeof(tr->arr_index_cur));

                bt.current->symtable[ConstDef->ident].val_p = val;
                
                // 接下来处理数组的初始化
                ConstDef->constinitval->accept(this);
                tr->arr_cur = NULL;
            }
            else if (tr->is_global) {
                value_ptr value_g = tr->NewValue();
                value_g->ty.tag = KOOPA_TYPE_POINTER;
                value_g->ty.data.pointer.base = new type_kind;
                // val->ty.tag = KOOPA_TYPE_ARRAY;
                // tr->arr.clear();
                type_kind *type = value_g->ty.data.pointer.base;
                // tr->arr.push_back(type);
                for (size_t i = 0; i < length.size(); i ++ ) {
                    type->tag = KOOPA_TYPE_ARRAY;
                    type->data.array.len = length[i];
                    type->data.array.base = new type_kind;
                    type = type->data.array.base;
                    // tr->arr.push_back(type);
                }
                type->tag = KOOPA_TYPE_POINTER;
                type->data.pointer.base = new type_kind;
                type->data.pointer.base->tag = KOOPA_TYPE_INT32;
                

                value_g->name = "@" + ConstDef->ident + "_" + std::to_string(rep_map[ConstDef->ident]);
                value_g->kind.tag = IR_GLOBAL_ALLOC;
                value_g->kind.data.global_alloc.init = (value_ptr)ConstDef->constinitval->accept(this);
                
                tr->AddGlobalValue(value_g);
                bt.current->symtable[ConstDef->ident].val_p = value_g;
            }

        }
        return NULL;
    }
    void *visit(ConstInitValAST *ConstInitVal) {
        if (ConstInitVal->d_type == ConstInitValAST::VALUE) {
            if (!tr->is_global) {
                // value_ptr store = tr->NewValue();
                // store->kind.tag = IR_STORE;
                // store->kind.data.store.dest = tr->arr_cur;
                // store->kind.data.store.value = new value;
                // store->kind.data.store.value->kind.tag = IR_INTEGER;
                // store->kind.data.store.value->kind.data.integer.value = ConstInitVal->constexp->get_value();
                // tr->AddValue(store);
                // return NULL;
                
                value_ptr store = tr->NewValue();
                store->kind.tag = IR_STORE;
                value_ptr dest = tr->arr_cur;
                if (dest->ty.data.pointer.base->tag == KOOPA_TYPE_INT32) {
                    store->kind.data.store.dest = dest;
                    store->kind.data.store.value = new value;
                    store->kind.data.store.value->kind.data.integer.value = ConstInitVal->constexp->get_value();
                    tr->AddValue(store);
                    return NULL;
                }
                int dim = tr->arr.size() - 1 - tr->GetDim(*dest->ty.data.pointer.base);
                // std::vector<value_ptr> arr;
                // arr.push_back(dest);
                // while (dest->ty.tag != KOOPA_TYPE_POINTER) {
                while (dest->ty.data.pointer.base->tag != KOOPA_TYPE_POINTER) {
                    // printf("iiii\n");
                    value_ptr getelem = tr->NewValue();
                    // getelem->ty = *dest->ty.data.array.base;
                    getelem->ty.tag = KOOPA_TYPE_POINTER;
                    getelem->ty.data.pointer.base = dest->ty.data.pointer.base->data.array.base;
                    getelem->kind.tag = IR_GET_ELEM_PTR;
                    getelem->kind.data.get_elem_ptr.index = new value;
                    getelem->kind.data.get_elem_ptr.index->kind.tag = IR_INTEGER;
                    // dim = tr->GetDim(getelem->ty);
                    getelem->kind.data.get_elem_ptr.index->kind.data.integer.value = tr->arr_index_cur[dim];
                    getelem->kind.data.get_elem_ptr.src = dest;
                    dim ++;
                    tr->AddValue(getelem);
                    dest = getelem;
                    // arr.push_back(dest);
                    tr->arr_cur = getelem;
                }
                tr->arr_cur = tr->arr_cur->kind.data.get_elem_ptr.src;
                while (dim >=1 && ++ tr->arr_index_cur[dim - 1] == tr->arr[dim - 1]->data.array.len) {
                    tr->arr_index_cur[dim - 1] = 0;
                    dim --;
                    tr->arr_cur = tr->arr_cur->kind.data.get_elem_ptr.src;
                }
                // tr->arr_index_cur[dim - 1] ++;
                store->kind.data.store.dest = dest;
                store->kind.data.store.value = new value;
                store->kind.data.store.value->kind.data.integer.value = ConstInitVal->constexp->get_value();
                tr->AddValue(store);
                return NULL;
            }
            else {
                value_ptr val = tr->NewValue();
                val->kind.tag = IR_INTEGER;
                val->kind.data.integer.value = ConstInitVal->constexp->get_value();
                return val;
            }
        }
        else if (ConstInitVal->d_type == ConstInitValAST::ARRAY) {
            if (!tr->is_global) {
                if (ConstInitVal->constinitvals != NULL)
                    ConstInitVal->constinitvals->accept(this);
                return NULL;
            }
            else {
                value_ptr aggregate = tr->NewValue();
                aggregate->kind.tag = IR_AGGREGATE;
                
                aggregate->kind.data.aggregate.elems = (slice_ptr)ConstInitVal->constinitvals->accept(this);
                return aggregate;
            }
        }
        assert(false);
        return NULL;
    }
    void *visit(ConstInitValsAST *ConstInitVals) {
        tr->is_array_init = true;
        if (!tr->is_global) {
            for (size_t i = 0; i < ConstInitVals->constinitvals.size(); i ++ ) {
                ConstInitVals->constinitvals[i]->accept(this);
            }
        }
        else {
            slice_ptr s = new slice;
            s->len = 0;
            for (size_t i = 0; i < ConstInitVals->constinitvals.size(); i ++ ) {
                value_ptr val = (value_ptr)ConstInitVals->constinitvals[i]->accept(this);
                s->buffer.push_back(val);
                s->len ++;
            }
            return s;
        }
        tr->is_array_init = false;
        return NULL;
    }
    void *visit(ConstExpsAST *ConstExps) {
        static std::vector<int> length;
        length.clear();
        for (size_t i = 0; i < ConstExps->constexps.size(); i ++ ) {
            length.push_back(ConstExps->constexps[i]->get_value());
        }
        return &length;
        return NULL;
    }
    void *visit(VarDeclAST *VarDecl) {
        if (VarDecl->btype == "int") tr->ty_cur.tag = KOOPA_TYPE_INT32;
        else {
            printf("There is an exception in visit of VarDecl!\n");
            assert(false);
        }
        VarDecl->vardefs->accept(this);
        return NULL;
    }
    void *visit(VarDefsAST *VarDefs) {
        for (size_t i = 0; i < VarDefs->vardefs.size(); i ++ ) {
            VarDefs->vardefs[i]->accept(this);
        }
        return NULL;
    }
    void *visit(VarDefAST *VarDef) {
        if (VarDef->d_type == VarDefAST::VALUE) {
            if (VarDef->type == VarDefAST::NO_VALUE) {
                if (tr->ty_cur.tag == KOOPA_TYPE_INT32) {
                    // 首先将变量插入到符号表中
                    bt.current->symtable[VarDef->ident].ty.tag = I32;
                    bt.current->symtable[VarDef->ident].value = 0;
                    bt.current->symtable[VarDef->ident].kind = VAR;
                    bt.current->symtable[VarDef->ident].has_value = false;
                    rep_map[VarDef->ident] += 1;

                    if (tr->is_global == false) {
                        // 插入一个alloc指令，将变量分配出来
                        value_ptr val_dest = tr->NewValue();
                        val_dest->kind.tag = IR_ALLOC;
                        if (tr->ty_cur.tag == KOOPA_TYPE_INT32) {
                            // val_dest->kind.data.alloc.ty.tag = KOOPA_TYPE_INT32;
                            // val_dest->ty.tag = KOOPA_TYPE_INT32;
                            val_dest->ty.tag = KOOPA_TYPE_POINTER;
                            val_dest->ty.data.pointer.base = new type_kind;
                            val_dest->ty.data.pointer.base->tag = KOOPA_TYPE_INT32;
                        }
                        else {
                            printf("There is an exception in visit of VarDef!\n");
                            assert(false);
                        }

                        val_dest->name = "@" + VarDef->ident + "_" + std::to_string(rep_map[VarDef->ident]);
                        std::string str = "@" + VarDef->ident + "_" + std::to_string(rep_map[VarDef->ident]);
                        val_dest->kind.data.alloc.name = new char;
                        for (size_t i = 0; i < str.size(); i ++ ) {
                            val_dest->kind.data.alloc.name[i] = str[i];
                        }
                        val_dest->kind.data.alloc.name[str.size()] = '\0';
                        // val_dest->kind.data.alloc.name = "@" + VarDef->ident;
                        tr->AddValue(val_dest);

                        bt.current->symtable[VarDef->ident].val_p = (void*)val_dest;
                    }
                    else {
                        // 创建一个全局value.
                        value_ptr global_val = tr->NewValue();
                        global_val->ty.tag = KOOPA_TYPE_POINTER;
                        global_val->ty.data.pointer.base = new type_kind;
                        global_val->ty.data.pointer.base->tag = tr->ty_cur.tag;
                        // global_val->ty.tag = tr->ty_cur.tag;
                        global_val->name = "@" + VarDef->ident + "_" + std::to_string(rep_map[VarDef->ident]);
                        global_val->kind.tag = IR_GLOBAL_ALLOC;
                        global_val->kind.data.global_alloc.init = tr->NewValue();
                        global_val->kind.data.global_alloc.init->ty.tag = tr->ty_cur.tag;
                        global_val->kind.data.global_alloc.init->kind.tag = IR_INTEGER;
                        global_val->kind.data.global_alloc.init->kind.data.integer.value = 0;
                        tr->AddGlobalValue(global_val);
                        bt.current->symtable[VarDef->ident].val_p = (void*)global_val;
                    }
                }
                else {
                    printf("There is an exception in visit of VarDef!\n");
                    assert(false);
                }
            }
            else if (VarDef->type == VarDefAST::HAS_VALUE) {

                if (tr->ty_cur.tag == KOOPA_TYPE_INT32) {
                    // 首先将变量添加到符号表中
                    bt.current->symtable[VarDef->ident].ty.tag = I32;
                    bt.current->symtable[VarDef->ident].value = 0;
                    bt.current->symtable[VarDef->ident].kind = VAR;
                    bt.current->symtable[VarDef->ident].has_value = true;
                    rep_map[VarDef->ident] += 1;

                    if (tr->is_global == false) {
                        // 首先插入一个alloc指令，将变量分配出来
                        value_ptr val_dest = tr->NewValue();
                        val_dest->kind.tag = IR_ALLOC;
                        if (tr->ty_cur.tag == KOOPA_TYPE_INT32) {
                            // val_dest->kind.data.alloc.ty.tag = KOOPA_TYPE_INT32;
                            // val_dest->ty.tag = KOOPA_TYPE_INT32;
                            val_dest->ty.tag = KOOPA_TYPE_POINTER;
                            val_dest->ty.data.pointer.base = new type_kind;
                            val_dest->ty.data.pointer.base->tag = KOOPA_TYPE_INT32;
                        }
                        else {
                            printf("There is an exception in visit of VarDef!\n");
                            assert(false);
                        }

                        val_dest->name = "@" + VarDef->ident + "_" + std::to_string(rep_map[VarDef->ident]);
                        std::string str = "@" + VarDef->ident + "_" + std::to_string(rep_map[VarDef->ident]);
                        val_dest->kind.data.alloc.name = new char;
                        for (size_t i = 0; i < str.size(); i ++ ) {
                            val_dest->kind.data.alloc.name[i] = str[i];
                        }
                        val_dest->kind.data.alloc.name[str.size()] = '\0';
                        tr->AddValue(val_dest);

                        // 将变量添加到符号表中
                        
                        bt.current->symtable[VarDef->ident].val_p = (void*)val_dest;

                        // 然后计算该变量的值
                        value_ptr val = (value_ptr)VarDef->initval->accept(this);

                        // 将该计算出来的值store给变量
                        value_ptr val_store = tr->NewValue();
                        val_store->kind.tag = IR_STORE;
                        val_store->kind.data.store.dest = val_dest;
                        val_store->kind.data.store.value = val;
                        tr->AddValue(val_store);
                    }
                    else {
                        // 创建一个全局value.
                        value_ptr global_val = tr->NewValue();
                        global_val->ty.tag = KOOPA_TYPE_POINTER;
                        global_val->ty.data.pointer.base = new type_kind;
                        global_val->ty.data.pointer.base->tag = tr->ty_cur.tag;
                        // global_val->ty.tag = tr->ty_cur.tag;
                        global_val->name = "@" + VarDef->ident + "_" + std::to_string(rep_map[VarDef->ident]);
                        global_val->kind.tag = IR_GLOBAL_ALLOC;
                        global_val->kind.data.global_alloc.init = tr->NewValue();
                        global_val->kind.data.global_alloc.init->ty.tag = tr->ty_cur.tag;
                        global_val->kind.data.global_alloc.init->kind.tag = IR_INTEGER;
                        global_val->kind.data.global_alloc.init->kind.data.integer.value = VarDef->initval->get_value();
                        tr->AddGlobalValue(global_val);
                        bt.current->symtable[VarDef->ident].val_p = (void*)global_val;
                    }
                }
                else {
                    assert(false);
                }
            }
            else {
                assert(false);
            }
        }
        else if (VarDef->d_type == VarDefAST::ARRAY) {
            // printf("There is a define of array.\n");
            // 将该数组类型加入符号表
            bt.current->symtable[VarDef->ident].has_value = false;
            bt.current->symtable[VarDef->ident].kind = VAR;
            
            rep_map[VarDef->ident] ++;

            std::vector<int> length = *(std::vector<int>*)VarDef->constexps->accept(this);
            tr->arr_len = length[0];

            type_t *type_p = &bt.current->symtable[VarDef->ident].ty;
            
            for (size_t i = 0; i < length.size(); i ++ ) {
                type_p->tag = ARRAY;
                type_p->data = new type_t;
                type_p = type_p->data;
            }
            type_p->tag = POINTER;
            type_p->data = new type_t;
            type_p->data->tag = I32;
            
            if (!tr->is_global) {
                
                value_ptr val = tr->NewValue();
                val->ty.tag = KOOPA_TYPE_POINTER;
                val->ty.data.pointer.base = new type_kind;
                tr->arr.clear();
                memset(tr->arr_index_cur, 0, sizeof(tr->arr_index_cur));
                // val->ty.tag = KOOPA_TYPE_ARRAY;
                type_kind *type = val->ty.data.pointer.base;
                tr->arr.push_back(type);
                for (size_t i = 0; i < length.size(); i ++ ) {
                    type->tag = KOOPA_TYPE_ARRAY;
                    type->data.array.len = length[i];
                    type->data.array.base = new type_kind;
                    type = type->data.array.base;
                    tr->arr.push_back(type);
                }
                type->tag = KOOPA_TYPE_POINTER;
                type->data.pointer.base = new type_kind;
                type->data.pointer.base->tag = KOOPA_TYPE_INT32;
                val->name = "@" + VarDef->ident + '_' + std::to_string(rep_map[VarDef->ident]);
                val->kind.tag = IR_ALLOC;
                tr->AddValue(val);
                tr->arr_cur = val;

                bt.current->symtable[VarDef->ident].val_p = val;
                
                // 接下来处理数组的初始化
                
                if (VarDef->type == VarDefAST::HAS_VALUE) {
                    VarDef->initval->accept(this);
                    bt.current->symtable[VarDef->ident].has_value = true;
                }
                tr->arr_cur = NULL;
            }
            else if (tr->is_global) {
                value_ptr value_g = tr->NewValue();
                value_g->ty.tag = KOOPA_TYPE_POINTER;
                value_g->ty.data.pointer.base = new type_kind;
                // val->ty.tag = KOOPA_TYPE_ARRAY;
                type_kind *type = value_g->ty.data.pointer.base;
                for (size_t i = 0; i < length.size(); i ++ ) {
                    type->tag = KOOPA_TYPE_ARRAY;
                    type->data.array.len = length[i];
                    type->data.array.base = new type_kind;
                    type = type->data.array.base;
                }
                type->tag = KOOPA_TYPE_POINTER;
                type->data.pointer.base = new type_kind;
                type->data.pointer.base->tag = KOOPA_TYPE_INT32;
                value_g->name = "@" + VarDef->ident + "_" + std::to_string(rep_map[VarDef->ident]);
                value_g->kind.tag = IR_GLOBAL_ALLOC;

                if (VarDef->type == VarDefAST::HAS_VALUE) {
                    value_g->kind.data.global_alloc.init = (value_ptr)VarDef->initval->accept(this);
                }
                else {
                    value_ptr zero_init = tr->NewValue();
                    zero_init->kind.tag = IR_ZERO_INIT;
                    value_g->kind.data.global_alloc.init = zero_init;
                }
                tr->AddGlobalValue(value_g);
                bt.current->symtable[VarDef->ident].val_p = value_g;
            }
        }
        return NULL;
    }
    void *visit(InitValAST *InitVal) {
        if (InitVal->d_type == InitValAST::VALUE) {
            if (!tr->is_global) {
                if (!tr->is_array_init) {
                    value_ptr val = (value_ptr)InitVal->exp->accept(this);
                    return val;
                }
                else {
                    // printf("There is init of array!\n");
                    value_ptr store = tr->NewValue();
                    store->kind.tag = IR_STORE;
                    value_ptr dest = tr->arr_cur;
                    if (dest->ty.data.pointer.base->tag == KOOPA_TYPE_INT32) {
                        store->kind.data.store.dest = dest;
                        store->kind.data.store.value = (value_ptr)InitVal->exp->accept(this);
                        tr->AddValue(store);
                        return NULL;
                    }
                    int dim = tr->arr.size() - 1 - tr->GetDim(*dest->ty.data.pointer.base);
                    // std::vector<value_ptr> arr;
                    // arr.push_back(dest);
                    // while (dest->ty.tag != KOOPA_TYPE_POINTER) {
                    while (dest->ty.data.pointer.base->tag != KOOPA_TYPE_POINTER) {
                        // printf("iiii\n");
                        value_ptr getelem = tr->NewValue();
                        // getelem->ty = *dest->ty.data.array.base;
                        getelem->ty.tag = KOOPA_TYPE_POINTER;
                        getelem->ty.data.pointer.base = dest->ty.data.pointer.base->data.array.base;
                        getelem->kind.tag = IR_GET_ELEM_PTR;
                        getelem->kind.data.get_elem_ptr.index = new value;
                        getelem->kind.data.get_elem_ptr.index->kind.tag = IR_INTEGER;
                        // dim = tr->GetDim(getelem->ty);
                        getelem->kind.data.get_elem_ptr.index->kind.data.integer.value = tr->arr_index_cur[dim];
                        getelem->kind.data.get_elem_ptr.src = dest;
                        dim ++;
                        tr->AddValue(getelem);
                        dest = getelem;
                        // arr.push_back(dest);
                        tr->arr_cur = getelem;
                    }
                    tr->arr_cur = tr->arr_cur->kind.data.get_elem_ptr.src;
                    while (dim >=1 && ++ tr->arr_index_cur[dim - 1] == tr->arr[dim - 1]->data.array.len) {
                        tr->arr_index_cur[dim - 1] = 0;
                        dim --;
                        tr->arr_cur = tr->arr_cur->kind.data.get_elem_ptr.src;
                    }
                    // tr->arr_index_cur[dim - 1] ++;
                    store->kind.data.store.dest = dest;
                    store->kind.data.store.value = (value_ptr)InitVal->exp->accept(this);
                    tr->AddValue(store);
                    return NULL;
                }
            }
            else {
                value_ptr val = tr->NewValue();
                val->kind.tag = IR_INTEGER;
                val->kind.data.integer.value = InitVal->exp->get_value();
                return val;
            }
        }
        else if (InitVal->d_type == InitValAST::ARRAY) {
            if (!tr->is_global) {
                if (InitVal->initvals != NULL)
                    InitVal->initvals->accept(this);
                return NULL;
            }
            else {
                value_ptr aggregate = tr->NewValue();
                aggregate->kind.tag = IR_AGGREGATE;
                
                aggregate->kind.data.aggregate.elems = (slice_ptr)InitVal->initvals->accept(this);
                return aggregate;
            }
        }
        return NULL;
    }
    void *visit(InitValsAST *InitVals) {
        tr->is_array_init = true;
        if (!tr->is_global) {
            for (size_t i = 0; i < InitVals->initvals.size(); i ++ ) {
                InitVals->initvals[i]->accept(this);
            }
        }
        else {
            slice_ptr s = new slice;
            s->len = 0;
            for (size_t i = 0; i < InitVals->initvals.size(); i ++ ) {
                value_ptr val = (value_ptr)InitVals->initvals[i]->accept(this);
                s->buffer.push_back(val);
                s->len ++;
            }
            return s;
        }
        tr->is_array_init = false;
        return NULL;
    }
    void *visit(ExpsAST *Exps) {
        static std::vector<value_ptr> indexs;
        indexs.clear();
        for (size_t i = 0; i < Exps->exps.size(); i ++ ) {
            indexs.push_back((value_ptr)Exps->exps[i]->accept(this));
        }
        return &indexs;
    }
    void *visit(StmtAST *Stmt) {
        tr->stmt_cur = Stmt;
        if (Stmt->type == StmtAST::OPENSTMT) {
            Stmt->openstmt->accept(this);
        } else if (Stmt->type == StmtAST::CLOSEDSTMT) {
            Stmt->closedstmt->accept(this);
        }
        return NULL;
    }
    void *visit(OpenStmtAST *OpenStmt) {
        value_ptr val = tr->NewValue();
        val->kind.tag = IR_BRANCH;
        val->kind.data.branch.cond = (value_ptr)OpenStmt->exp->accept(this);
        tr->AddValue(val);
        if (OpenStmt->type == OpenStmtAST::IF) {
            // 创建一个基本块，作为条件为true时所跳转的基本块
            basic_block_ptr bb = tr->NewBasicBlock();
            tr->AddBasicBlock(bb);
            tr->bb_cur = bb;
            val->kind.data.branch.true_bb = bb;
            bb->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            OpenStmt->stmt->accept(this);

            value_ptr val_j1 = tr->NewValue();
            val_j1->kind.tag = IR_JUMP;
            tr->AddValue(val_j1);

            // 创建一个新的基本块，作为分支指令创建的基本块的出口
            basic_block_ptr bb_end = tr->NewBasicBlock();
            tr->AddBasicBlock(bb_end);
            tr->bb_cur = bb_end;
            bb_end->name = "%b" + std::to_string(bb_id);
            bb_id ++;

            val->kind.data.branch.false_bb = bb_end;

            // 在创建的基本块的末尾加上跳转指令
            val_j1->kind.data.jump.target = bb_end;
        }
        else if (OpenStmt->type == OpenStmtAST::IF_ELSE) {
            // 创建一个新的基本块，作为条件为true时跳转的基本块
            basic_block_ptr bb_true = tr->NewBasicBlock();
            tr->AddBasicBlock(bb_true);
            tr->bb_cur = bb_true;
            val->kind.data.branch.true_bb = bb_true;
            bb_true->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            OpenStmt->closedstmt->accept(this);

            value_ptr val_j1 = tr->NewValue();
            val_j1->kind.tag = IR_JUMP;
            tr->AddValue(val_j1);

            // 创建一个基本块，作为条件为false时跳转的基本块
            basic_block_ptr bb_false = tr->NewBasicBlock();
            tr->AddBasicBlock(bb_false);
            tr->bb_cur = bb_false;
            val->kind.data.branch.false_bb = bb_false;
            bb_false->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            OpenStmt->openstmt->accept(this);

            value_ptr val_j2 = tr->NewValue();
            val_j2->kind.tag = IR_JUMP;
            tr->AddValue(val_j2);

            // 创建一个新的基本块，作为分支指令创建的基本块的出口
            
            basic_block_ptr bb_end = tr->NewBasicBlock();
            tr->AddBasicBlock(bb_end);
            tr->bb_cur = bb_end;
            bb_end->name = "%b" + std::to_string(bb_id);
            bb_id ++;

            // 在创建的基本块的末尾加上跳转指令
            
            val_j1->kind.data.jump.target = bb_end;

            
            val_j2->kind.data.jump.target = bb_end;
        }
        return NULL;
    }
    void *visit(ClosedStmtAST *ClosedStmt) {
        
        if (ClosedStmt->type == ClosedStmtAST::NONIF) {
            ClosedStmt->nonifstmt->accept(this);
        }
        else if (ClosedStmt->type == ClosedStmtAST::IF_ELSE) {
            // 创建一条新的branch指令
            value_ptr val = tr->NewValue();
            val->kind.tag = IR_BRANCH;
            val->kind.data.branch.cond = (value_ptr)ClosedStmt->exp->accept(this);
            tr->AddValue(val);
            // 创建条件为true时应该跳转的基本块
            basic_block_ptr bb_true = tr->NewBasicBlock();
            tr->AddBasicBlock(bb_true);
            tr->bb_cur = bb_true;
            val->kind.data.branch.true_bb = bb_true;
            bb_true->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            ClosedStmt->closedstmt_if->accept(this);
            // 在处理完true基本块后，在当前基本块中添加跳转指令（由上一个bb_end跳转到当前bb_end）
            value_ptr val_j1 = tr->NewValue();
            val_j1->kind.tag = IR_JUMP;
            tr->AddValue(val_j1);
            // 创建条件为false时应该跳转的基本块
            basic_block_ptr bb_false = tr->NewBasicBlock();
            tr->AddBasicBlock(bb_false);
            tr->bb_cur = bb_false;
            val->kind.data.branch.false_bb = bb_false;
            bb_false->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            ClosedStmt->closedstmt_else->accept(this);
            // 在处理完false基本块后，在当前基本块中添加跳转指令（由上一个bb_end跳转到当前bb_end）
            value_ptr val_j2 = tr->NewValue();
            val_j2->kind.tag = IR_JUMP;
            tr->AddValue(val_j2);
            // 创建收尾基本块，分支执行完之后将跳转到这里
            basic_block_ptr bb_end = tr->NewBasicBlock();
            tr->AddBasicBlock(bb_end);
            bb_end->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            tr->bb_cur = bb_end;

            // 在创建的基本块的末尾加上跳转指令
            val_j1->kind.data.jump.target = bb_end;
            val_j2->kind.data.jump.target = bb_end;
        }
        return NULL;
    }
    void *visit(NonIfStmtAST *NonIfStmt) {
        // 创建一个新的value类型（这里表示指令），并为其成员变量分配空间
        value_ptr val = tr->NewValue();
        
        if (NonIfStmt->type == NonIfStmtAST::STMT) {
            delete val;
            val = (value_ptr)NonIfStmt->lessstmt->accept(this);
        }
        else if (NonIfStmt->type == NonIfStmtAST::BLOCK) {
            NonIfStmt->block->accept(this);
        }
        else if (NonIfStmt->type == NonIfStmtAST::WHILE) {
            // 创建一个新的循环结点
            LoopTreeNode *ltn = new LoopTreeNode;
            ltn->father_loop = lt.current;
            lt.current = ltn;

            // 创建一个新的jump指令，跳转向循环体
            value_ptr val_j1 = tr->NewValue();
            tr->AddValue(val_j1);
            val_j1->kind.tag = IR_JUMP;

            // 创建三个基本块，分别作为循环条件、循环体、循环出口
            basic_block_ptr while_entry = tr->NewBasicBlock();
            while_entry->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            tr->AddBasicBlock(while_entry);
            basic_block_ptr while_body = tr->NewBasicBlock();
            while_body->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            tr->AddBasicBlock(while_body);
            basic_block_ptr end = tr->NewBasicBlock();
            end->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            tr->AddBasicBlock(end);

            val_j1->kind.data.jump.target = while_entry;

            ltn->loop_entry = while_entry;
            ltn->loop_body = while_body;
            ltn->loop_end = end;

            // 处理循环条件判断部分
            tr->bb_cur = while_entry;

            // 在while_entry的末尾添加br指令
            value_ptr val_br = tr->NewValue();
            val_br->kind.tag = IR_BRANCH;
            val_br->kind.data.branch.cond = (value_ptr)NonIfStmt->exp->accept(this);
            tr->AddValue(val_br);

            // 处理循环体部分
            tr->bb_cur = while_body;
            val_br->kind.data.branch.true_bb = while_body;

            NonIfStmt->stmt->accept(this);
            // 在循环体的末尾添加Jump指令
            value_ptr val_j2 = tr->NewValue();
            tr->AddValue(val_j2);
            val_j2->kind.tag = IR_JUMP;
            val_j2->kind.data.jump.target = while_entry;

            tr->bb_cur = end;
            val_br->kind.data.branch.false_bb = end;

            // 该循环已经处理完毕，当前循环退出到父循环
            lt.current = ltn->father_loop;
            delete ltn;
            ltn = NULL;
        }
        else if (NonIfStmt->type == NonIfStmtAST::FOR) {
            // 创建一个新的循环结点
            LoopTreeNode *ltn = new LoopTreeNode;
            ltn->father_loop = lt.current;
            lt.current = ltn;

            

            BlockTreeNode *btn = new BlockTreeNode;
            btn->father_block = bt.current;
            if (bt.current != bt.root) {
                btn->symtable = bt.current->symtable;
                btn->father_block = bt.current;
            }
            bt.current = btn;

            NonIfStmt->blockitem->accept(this);

            

            // 创建一个新的jump指令，跳转向循环条件判断部分
            value_ptr val_j1 = tr->NewValue();
            tr->AddValue(val_j1);
            val_j1->kind.tag = IR_JUMP;

            // 创建三个基本块，分别作为循环条件、循环体、循环出口
            basic_block_ptr while_entry = tr->NewBasicBlock();
            while_entry->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            tr->AddBasicBlock(while_entry);
            basic_block_ptr while_body = tr->NewBasicBlock();
            while_body->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            tr->AddBasicBlock(while_body);
            basic_block_ptr end = tr->NewBasicBlock();
            end->name = "%b" + std::to_string(bb_id);
            bb_id ++;
            tr->AddBasicBlock(end);

            val_j1->kind.data.jump.target = while_entry;

            ltn->loop_entry = while_entry;
            ltn->loop_body = while_body;
            ltn->loop_end = end;

            // 处理循环条件判断部分
            tr->bb_cur = while_entry;

            // 在while_entry的末尾添加br指令
            value_ptr val_br = tr->NewValue();
            val_br->kind.tag = IR_BRANCH;
            val_br->kind.data.branch.cond = (value_ptr)NonIfStmt->exp->accept(this);
            tr->AddValue(val_br);
            
            // 处理循环体部分
            tr->bb_cur = while_body;
            val_br->kind.data.branch.true_bb = while_body;

            NonIfStmt->stmt->accept(this);
            NonIfStmt->lessstmt->accept(this);
            // 在循环体的末尾添加Jump指令
            value_ptr val_j2 = tr->NewValue();
            tr->AddValue(val_j2);
            val_j2->kind.tag = IR_JUMP;
            val_j2->kind.data.jump.target = while_entry;

            tr->bb_cur = end;
            val_br->kind.data.branch.false_bb = end;

            bt.current = btn->father_block;
            btn->symtable.clear();
            delete btn;
            btn = NULL;
            // 该循环已经处理完毕，当前循环退出到父循环
            lt.current = ltn->father_loop;
            delete ltn;
            ltn = NULL;
        }

        return val;
    }
    void *visit(LessStmtAST *LessStmt) {
        value_ptr val = tr->NewValue();
        if (LessStmt->type == LessStmtAST::RETURN) {
            
            // 目前只处理return指令
            val->kind.tag = IR_RETURN;

            if (LessStmt->exp != NULL) {
                value_ptr val_ret = (value_ptr)LessStmt->exp->accept(this);
                val->kind.data.ret.value = val_ret;
            }
            else {
                val->kind.data.ret.value = NULL;
            }
            // 将该指令添加到当前基本块中
            tr->AddValue(val);
        }
        else if (LessStmt->type == LessStmtAST::ASSIGN) {
            void *ptr = LessStmt->lval->accept(this);
            std::string ident = LessStmt->lval->get_name();
            if (bt.current->symtable.count(ident) == 0) {
                value_ptr val = tr->IsParam(ident);
                if (val != NULL) {
                    if (ptr == NULL) return val;
                    std::vector<value_ptr> indexs = *(std::vector<value_ptr>*)ptr;
                    value_ptr getptr = tr->NewValue();
                    getptr->ty = val->ty;
                    getptr->kind.tag = IR_GET_PTR;
                    getptr->kind.data.get_ptr.index = indexs[0];
                    getptr->kind.data.get_ptr.src = val;
                    tr->AddValue(getptr);
                    val = getptr;
                    
                    int i = 1;
                    // while (val->ty.tag == KOOPA_TYPE_ARRAY) {
                    while (val->ty.data.pointer.base->tag == KOOPA_TYPE_ARRAY) {
                        value_ptr getelem = tr->NewValue();
                        getelem->ty.tag = KOOPA_TYPE_POINTER;
                        getelem->ty.data.pointer.base = val->ty.data.pointer.base->data.array.base;
                        // getelem->ty = *val->ty.data.array.base;
                        getelem->kind.tag = IR_GET_ELEM_PTR;
                        getelem->kind.data.get_elem_ptr.index = indexs[i];
                        getelem->kind.data.get_elem_ptr.src = val;
                        tr->AddValue(getelem);
                        val = getelem;
                        i ++;
                    }
                    value_ptr store = tr->NewValue();
                    store->kind.tag = IR_STORE;
                    store->kind.data.store.value = (value_ptr)LessStmt->exp->accept(this);
                    store->kind.data.store.dest = val;
                    tr->AddValue(store);
                    return store;
                }
                else {
                    printf("There is an undefined variable was used!\n");
                    assert(false);
                }
            }


            std::vector<value_ptr> indexs;
            if (ptr != NULL) indexs = *(std::vector<value_ptr>*)ptr;
            
            value_ptr arr = (value_ptr)bt.current->symtable[ident].val_p;
            val->kind.tag = IR_STORE;
            val->kind.data.store.value = (value_ptr)LessStmt->exp->accept(this);
            for (size_t i = 0; i < indexs.size(); i ++ ) {
                value_ptr getelem = tr->NewValue();
                // getelem->ty = *arr->ty.data.array.base;
                getelem->ty.tag = KOOPA_TYPE_POINTER;
                getelem->ty.data.pointer.base = arr->ty.data.pointer.base->data.array.base;
                getelem->kind.tag = IR_GET_ELEM_PTR;
                getelem->kind.data.get_elem_ptr.index = indexs[i];
                getelem->kind.data.get_elem_ptr.src = arr;
                tr->AddValue(getelem);
                arr = getelem;
            }
            // val->kind.data.store.dest = (value_ptr)bt.current->symtable[ident].val_p;
            val->kind.data.store.dest = arr;
            bt.current->symtable[ident].has_value = true;
            tr->AddValue(val);
        }
        else if (LessStmt->type == LessStmtAST::VOID) {
            if (LessStmt->exp != NULL)
                LessStmt->exp->accept(this);
            return NULL;
        }
        else if (LessStmt->type == LessStmtAST::BREAK) {
            if (lt.current == lt.root) {
                printf("error:'break' statement is mistakenly used outside the loop structure.\n");
                assert(false);
            }
            value_ptr val = tr->NewValue();
            val->kind.tag = IR_JUMP;
            val->kind.data.jump.target = lt.current->loop_end;
            tr->AddValue(val);
        }
        else if (LessStmt->type == LessStmtAST::CONTINUE) {
            if (lt.current == lt.root) {
                printf("error:'continue' statement is mistakenly used outside the loop structrue.\n");
                assert(false);
            }
            value_ptr val = tr->NewValue();
            val->kind.tag = IR_JUMP;
            val->kind.data.jump.target = lt.current->loop_entry;
            tr->AddValue(val);
        }
        return val;
    }
    void *visit(LValAST *lval) {
        if (lval->d_type == LValAST::VALUE) {
            return NULL;
        }
        else if (lval->d_type == LValAST::ARRAY) {
            static std::vector<value_ptr> indexs;
            indexs.clear();
            indexs = *(std::vector<value_ptr>*)lval->exps->accept(this);
            return &indexs;
        }
        assert(false);
        return NULL;
    }
    void *visit(ExpAST *Exp) {
        value_ptr val_ret = (value_ptr)Exp->lorexp->accept(this);
        return val_ret;
    }
    void *visit(PrimaryExpAST *PrimaryExp) {
        if (PrimaryExp->type == PrimaryExpAST::NUM) {
            value_ptr val = new value;
            val->kind.tag = IR_INTEGER;
            val->kind.data.integer.value = PrimaryExp->number;
            return val;
        }
        else if (PrimaryExp->type == PrimaryExpAST::EXP) {
            value_ptr val = (value_ptr)PrimaryExp->exp->accept(this);
            return val;
        }
        else if (PrimaryExp->type == PrimaryExpAST::LVAL) {
            void *ptr = PrimaryExp->lval->accept(this);
            std::string ident = PrimaryExp->lval->get_name();
            if (bt.current->symtable.count(ident) == 0) {
                value_ptr val = tr->IsParam(ident);
                if (val != NULL) {
                    if (ptr == NULL) return val;
                    std::vector<value_ptr> indexs = *(std::vector<value_ptr>*)ptr;
                    value_ptr getptr = tr->NewValue();
                    getptr->ty = val->ty;
                    getptr->kind.tag = IR_GET_PTR;
                    getptr->kind.data.get_ptr.index = indexs[0];
                    getptr->kind.data.get_ptr.src = val;
                    tr->AddValue(getptr);
                    val = getptr;
                    
                    int i = 1;
                    // while (val->ty.tag == KOOPA_TYPE_ARRAY) {
                    while (val->ty.data.pointer.base->tag == KOOPA_TYPE_ARRAY && i < indexs.size()) {
                        value_ptr getelem = tr->NewValue();
                        getelem->ty.tag = KOOPA_TYPE_POINTER;
                        getelem->ty.data.pointer.base = val->ty.data.pointer.base->data.array.base;
                        // getelem->ty = *val->ty.data.array.base;
                        getelem->kind.tag = IR_GET_ELEM_PTR;
                        getelem->kind.data.get_elem_ptr.index = indexs[i];
                        getelem->kind.data.get_elem_ptr.src = val;
                        tr->AddValue(getelem);
                        val = getelem;
                        i ++;
                    }
                    value_ptr load = tr->NewValue();
                    load->ty = *val->ty.data.pointer.base;
                    load->kind.tag = IR_LOAD;
                    load->kind.data.load.src = val;
                    tr->AddValue(load);
                    return load;
                }
                else {
                    printf("There is an undefined variable was used!\n");
                    assert(false);
                }
            }
            if (bt.current->symtable[ident].kind == CON) {
                if (bt.current->symtable[ident].ty.tag == I32) {
                    value_ptr val = new value;
                    val->kind.tag = IR_INTEGER;
                    val->kind.data.integer.value = bt.current->symtable[ident].value;
                    return val;
                }
                else if (bt.current->symtable[ident].ty.tag == ARRAY) {
                    if (ptr == NULL) {
                        value_ptr arr = (value_ptr)bt.current->symtable[ident].val_p;
                        type_kind type = *arr->ty.data.pointer.base;
                        while (type.tag == KOOPA_TYPE_ARRAY) {
                            value_ptr getelem = tr->NewValue();
                            getelem->ty = *type.data.array.base;
                            getelem->kind.tag = IR_GET_ELEM_PTR;
                            getelem->kind.data.get_elem_ptr.index = new value;
                            getelem->kind.data.get_elem_ptr.index->kind.tag = IR_INTEGER;
                            getelem->kind.data.get_elem_ptr.index->kind.data.integer.value = 0;
                            getelem->kind.data.get_elem_ptr.src = arr;
                            tr->AddValue(getelem);
                            type = getelem->ty;
                            arr = getelem;
                        }
                        return arr;
                    }
                    // std::vector<value_ptr> indexs = *(std::vector<value_ptr>*)PrimaryExp->lval->accept(this);
                    std::vector<value_ptr> indexs = *(std::vector<value_ptr>*)ptr;
                    // if (indexs) assert(false);
                    value_ptr arr = (value_ptr)bt.current->symtable[ident].val_p;
                    int i = 0;
                    // while (arr->ty.tag == KOOPA_TYPE_ARRAY) {
                    while (arr->ty.data.pointer.base->tag == KOOPA_TYPE_ARRAY) {
                        value_ptr getelem = tr->NewValue();
                        // getelem->ty = *arr->ty.data.array.base;
                        getelem->ty.tag = KOOPA_TYPE_POINTER;
                        getelem->ty.data.pointer.base = arr->ty.data.pointer.base->data.array.base;
                        getelem->kind.tag = IR_GET_ELEM_PTR;
                        getelem->kind.data.get_elem_ptr.index = indexs[i];
                        getelem->kind.data.get_elem_ptr.src = arr;
                        tr->AddValue(getelem);
                        arr = getelem;
                        i ++;
                    }
                    value_ptr load = tr->NewValue();
                    load->ty = arr->ty;
                    load->kind.tag = IR_LOAD;
                    load->kind.data.load.src = arr;
                    tr->AddValue(load);
                    return load;
                }
            }
            else if (bt.current->symtable[ident].kind == VAR) {
                if (bt.current->symtable[ident].ty.tag == I32) {
                    value_ptr val = tr->NewValue();
                    val->kind.tag = IR_LOAD;
                    val->kind.data.load.src = (value_ptr)bt.current->symtable[ident].val_p;
                    val->ty = *val->kind.data.load.src->ty.data.pointer.base;
                    tr->AddValue(val);
                    return val;
                }
                else if (bt.current->symtable[ident].ty.tag == ARRAY) {
                    if (ptr == NULL) {
                        value_ptr arr = (value_ptr)bt.current->symtable[ident].val_p;
                        type_kind type = *arr->ty.data.pointer.base;
                        while (type.tag == KOOPA_TYPE_ARRAY) {
                            value_ptr getelem = tr->NewValue();
                            getelem->ty = *type.data.array.base;
                            getelem->kind.tag = IR_GET_ELEM_PTR;
                            getelem->kind.data.get_elem_ptr.index = new value;
                            getelem->kind.data.get_elem_ptr.index->kind.tag = IR_INTEGER;
                            getelem->kind.data.get_elem_ptr.index->kind.data.integer.value = 0;
                            getelem->kind.data.get_elem_ptr.src = arr;
                            tr->AddValue(getelem);
                            type = getelem->ty;
                            arr = getelem;
                        }
                        return arr;
                    }
                    // std::vector<value_ptr> indexs = *(std::vector<value_ptr>*)PrimaryExp->lval->accept(this);
                    std::vector<value_ptr> indexs = *(std::vector<value_ptr>*)ptr;
                    // if (indexs) assert(false);
                    value_ptr arr = (value_ptr)bt.current->symtable[ident].val_p;
                    int i = 0;
                    // while (arr->ty.tag == KOOPA_TYPE_ARRAY) {
                    while (arr->ty.data.pointer.base->tag == KOOPA_TYPE_ARRAY) {
                        value_ptr getelem = tr->NewValue();
                        // getelem->ty = *arr->ty.data.array.base;
                        getelem->ty.tag = KOOPA_TYPE_POINTER;
                        // getelem->ty.data.pointer.base = new type_kind;
                        getelem->ty.data.pointer.base = arr->ty.data.pointer.base->data.array.base;
                        getelem->kind.tag = IR_GET_ELEM_PTR;
                        getelem->kind.data.get_elem_ptr.index = indexs[i];
                        getelem->kind.data.get_elem_ptr.src = arr;
                        tr->AddValue(getelem);
                        arr = getelem;
                        i ++;
                    }
                    value_ptr load = tr->NewValue();
                    load->ty = arr->ty;
                    load->kind.tag = IR_LOAD;
                    load->kind.data.load.src = arr;
                    tr->AddValue(load);
                    return load;
                }
                else assert(false);
            }
            else {
                printf("Here!!!\n");
                printf("There is an undefined variable was used!\n");
                assert(false);
            }
        }
        else {
            printf("There is a exception in visit of PrimaryExp!\n");
            return NULL;
        }
        return NULL;
    }
    void *visit(UnaryExpAST *UnaryExp) {
        if (UnaryExp->type == UnaryExpAST::NAN) {
            value_ptr val = (value_ptr)UnaryExp->primaryexp->accept(this);
            return val;
        }
        else if (UnaryExp->type == UnaryExpAST::EXP) {
            // 如果该结点设计运算，将产生指令
            // 表达式之前的一元"+"运算符不做处理
            if (UnaryExp->unaryop == "+") {
                value_ptr val = (value_ptr)UnaryExp->unaryexp->accept(this);
                return val;
            }
            
            // 新建一个value类型，并为其成员变量分配内存空间
            value_ptr val = tr->NewValue();

            val->kind.tag = IR_BINARY;
            // 设置二元运算指令的opcode
            if (UnaryExp->unaryop == "-") val->kind.data.binary.op = SUB;
            else if (UnaryExp->unaryop == "!") val->kind.data.binary.op = EQ;
            // 设置二元运算指令的第一个操作数
            val->kind.data.binary.lhs = new value;
            val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
            val->kind.data.binary.lhs->kind.data.integer.value = 0;
            // 设置二元运算指令的第二个操作数
            val->kind.data.binary.rhs = (value_ptr)UnaryExp->unaryexp->accept(this);

            // 将该指令添加到当前的基本块中
            tr->AddValue(val);
            
            return val;
        }
        else if (UnaryExp->type == UnaryExpAST::FUNC_NO_PARAMS) {
            value_ptr call = tr->NewValue();
            call->ty.tag = func_map[UnaryExp->ident]->ty.tag;
            call->kind.tag = IR_CALL;
            call->kind.data.call.callee = func_map[UnaryExp->ident];
            slice_ptr args = new slice;
            args->len = 0;
            call->kind.data.call.args = args;
            tr->AddValue(call);
            return call;
        }
        else if (UnaryExp->type == UnaryExpAST::FUNC_HAS_PARAMS) {
            value_ptr call = tr->NewValue();
            call->ty.tag = func_map[UnaryExp->ident]->ty.tag;
            call->kind.tag = IR_CALL;
            call->kind.data.call.callee = func_map[UnaryExp->ident];
            call->kind.data.call.args = (slice_ptr)UnaryExp->funcrparams->accept(this);
            tr->AddValue(call);
            return call;
        }
        else {
            printf("There is a exception in visit of UnaryExp!\n");
            return NULL;
        }
    }
    void *visit(FuncRParamsAST *FuncRParams) {
        slice_ptr params = new slice;
        for (size_t i = 0; i < FuncRParams->funcrparams.size(); i ++ ) {
            value_ptr param = (value_ptr)FuncRParams->funcrparams[i]->accept(this);
            tr->InsertParam(param, params);
        }
        return params;
    }
    void *visit(MulExpAST *MulExp) {
        if (MulExp->type == MulExpAST::NAN) {
            value_ptr val = (value_ptr)MulExp->unaryexp->accept(this);
            return val;
        }
        else {
            value_ptr val = tr->NewValue();

            val->kind.tag = IR_BINARY;
            
            if (MulExp->mulop == "*") val->kind.data.binary.op = MUL;
            else if (MulExp->mulop == "/") val->kind.data.binary.op = DIV;
            else if (MulExp->mulop == "%") val->kind.data.binary.op = MOD;
            else {
                printf("There is a exception in visit of MulExp!\n");
                assert(false);
            }

            val->kind.data.binary.lhs = (value_ptr)MulExp->mulexp->accept(this);
            val->kind.data.binary.rhs = (value_ptr)MulExp->unaryexp->accept(this);

            // 将该指令添加到当前的基本块中
            tr->AddValue(val);
            return val;
        }
    }
    void *visit(AddExpAST *AddExp) {
        if (AddExp->type == AddExpAST::NAN) {
            value_ptr val = (value_ptr)AddExp->mulexp->accept(this);
            return val;
        }
        else {
            value_ptr val = tr->NewValue();

            val->kind.tag = IR_BINARY;

            if (AddExp->addop == "+") val->kind.data.binary.op = ADD;
            else if (AddExp->addop == "-") val->kind.data.binary.op = SUB;
            else {
                printf("There is a exception in visit of AddExp!\n");
                assert(false);
            }

            val->kind.data.binary.lhs = (value_ptr)AddExp->addexp->accept(this);
            val->kind.data.binary.rhs = (value_ptr)AddExp->mulexp->accept(this);

            // 将该指令添加到当前的基本块中
            tr->AddValue(val);
            return val;
        }
    }
    void *visit(RelExpAST *RelExp) {
        if (RelExp->type == RelExpAST::NAN) {
            value_ptr val = (value_ptr)RelExp->addexp->accept(this);
            return val;
        }
        else {
            value_ptr val = tr->NewValue();

            val->kind.tag = IR_BINARY;

            if (RelExp->relop == "<") val->kind.data.binary.op = LT;
            else if (RelExp->relop == ">") val->kind.data.binary.op = GT;
            else if (RelExp->relop == "<=") val->kind.data.binary.op = LE;
            else if (RelExp->relop == ">=") val->kind.data.binary.op = GE;
            else {
                printf("There is a exception in visit of RelExp!\n");
                assert(false);
            }

            val->kind.data.binary.lhs = (value_ptr)RelExp->relexp->accept(this);
            val->kind.data.binary.rhs = (value_ptr)RelExp->addexp->accept(this);

            // 将该指令添加到当前的基本块中
            tr->AddValue(val);
            return val;
        }
    }
    void *visit(EqExpAST *EqExp) {
        if (EqExp->type == EqExpAST::NAN) {
            value_ptr val = (value_ptr)EqExp->relexp->accept(this);
            return val;
        }
        else {
            value_ptr val = tr->NewValue();

            val->kind.tag = IR_BINARY;

            if (EqExp->eqop == "==") val->kind.data.binary.op = EQ;
            else if (EqExp->eqop == "!=") val->kind.data.binary.op = NOT_EQ;
            else {
                printf("There is a exception in visit of RelExp!\n");
                assert(false);
            }

            val->kind.data.binary.lhs = (value_ptr)EqExp->eqexp->accept(this);
            val->kind.data.binary.rhs = (value_ptr)EqExp->relexp->accept(this);

            // 将该指令添加到当前的基本块中
            tr->AddValue(val);
            return val;
        }
    }
    void *visit(LAndExpAST *LAndExp) {
        if (LAndExp->type == LAndExpAST::NAN) {
            value_ptr val = (value_ptr)LAndExp->eqexp->accept(this);
            return val;
        }
        else {
            value_ptr l_val = tr->NewValue();
            l_val->kind.tag = IR_BINARY;
            l_val->kind.data.binary.op = NOT_EQ;
            l_val->kind.data.binary.lhs = new value;
            l_val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
            l_val->kind.data.binary.lhs->kind.data.integer.value = 0;
            l_val->kind.data.binary.rhs = (value_ptr)LAndExp->landexp->accept(this);

            // 将该指令添加到当前的基本块中
            tr->AddValue(l_val);

            value_ptr r_val = tr->NewValue();
            r_val->kind.tag = IR_BINARY;
            r_val->kind.data.binary.op = NOT_EQ;
            r_val->kind.data.binary.lhs = new value;
            r_val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
            r_val->kind.data.binary.lhs->kind.data.integer.value = 0;
            r_val->kind.data.binary.rhs = (value_ptr)LAndExp->eqexp->accept(this);

            tr->AddValue(r_val);

            value_ptr val = tr->NewValue();

            val->kind.tag = IR_BINARY;

            val->kind.data.binary.op = AND;

            val->kind.data.binary.lhs = l_val;
            val->kind.data.binary.rhs = r_val;

            // 将该指令添加到当前的基本块中
            tr->AddValue(val);
            return val;
        }
    }
    void *visit(LOrExpAST *LOrExp) {
        if (LOrExp->type == LOrExpAST::NAN) {
            value_ptr val = (value_ptr)LOrExp->landexp->accept(this);
            return val;
        }
        else {
            value_ptr l_val = tr->NewValue();
            l_val->kind.tag = IR_BINARY;
            l_val->kind.data.binary.op = NOT_EQ;
            l_val->kind.data.binary.lhs = new value;
            l_val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
            l_val->kind.data.binary.lhs->kind.data.integer.value = 0;
            l_val->kind.data.binary.rhs = (value_ptr)LOrExp->lorexp->accept(this);

            // 将该指令添加到当前的基本块中
            tr->AddValue(l_val);

            value_ptr r_val = tr->NewValue();
            r_val->kind.tag = IR_BINARY;
            r_val->kind.data.binary.op = NOT_EQ;
            r_val->kind.data.binary.lhs = new value;
            r_val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
            r_val->kind.data.binary.lhs->kind.data.integer.value = 0;
            r_val->kind.data.binary.rhs = (value_ptr)LOrExp->landexp->accept(this);

            tr->AddValue(r_val);

            value_ptr val = tr->NewValue();

            val->kind.tag = IR_BINARY;

            val->kind.data.binary.op = OR;

            val->kind.data.binary.lhs = l_val;
            val->kind.data.binary.rhs = r_val;

            // 将该指令添加到当前的基本块中
            tr->AddValue(val);
            return val;
        }
    }
};