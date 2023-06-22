#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <assert.h>
#include "../midend/xc.hpp"
// #include "AST.hpp"
class BaseAST;
class CompUnitAST;
class CompItemsAST;
class CompItemAST;

class FuncDefAST;
class FuncTypeAST;
class FuncFParamsAST;
class FuncFParamAST;
class FuncRParamsAST;

// 基本块类型
class BlockAST;
class BlockItemsAST;
class BlockItemAST;

// 变量声明
class DeclAST;
class ConstDeclAST;
class ConstDefsAST;
class ConstDefAST;
class ConstInitValAST;
class ConstExpAST;
class VarDeclAST;
class VarDefsAST;
class VarDefAST;
class InitValAST;

// 语句类型
class StmtAST;
class OpenStmtAST;
class ClosedStmtAST;
class NonIfStmtAST;
class LessStmtAST;

// 表达式类型
class ExpAST;
class PrimaryExpAST;
class UnaryExpAST;
class UnaryOpAST;
class MulExpAST;
class AddExpAST;
class RelExpAST;
class EqExpAST;
class LAndExpAST;
class LOrExpAST;

class Translate_Helper {
    public:
        // 当前所在的编译单元
        prog_ptr prog_cur;
        // 当前所在的函数
        func_ptr func_cur;
        // 当前所在的基本块（Koopa IR中的概念）
        basic_block_ptr bb_cur;
        // 当前应该处理的类型
        type_kind ty_cur;
        // 当前所处理的语句
        StmtAST *stmt_cur;
        // 当前函数参数的index
        int fp_index_cur;

        value_ptr NewValue() {
            // 新建一个value类型，并为其成员变量分配内存空间
            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;
            return val;
        }
        
        basic_block_ptr NewBasicBlock() {
            basic_block_ptr bb = new basic_block;
            bb->used_by = new slice;
            bb->used_by->len = 0;
            bb->params = new slice;
            bb->params->len = 0;
            bb->insts = new slice;
            bb->insts->len = 0;
            return bb;
        }

        func_ptr NewFuncion() {
            func_ptr func = new function;
            func->bbs = new slice;
            func->bbs->len = 0;
            func->params = new slice;
            func->params->len = 0;
            return func;
        }

        prog_ptr NewProgram() {
            prog_ptr prog = new program;
            prog->funcs = new slice;
            prog->funcs->len = 0;
            prog->values = new slice;
            prog->values->len = 0;
            return prog;
        }

        void AddFunc(func_ptr func) {
            prog_cur->funcs->buffer.push_back(func);
            prog_cur->funcs->len ++;
            prog_cur->funcs->kind = RISK_FUNCTION;
        }

        // 判断变量名为param_name的变量是否存在于当前函数的参数列表中，如果存在，返回该存数的value指针，否则返回NULL
        value_ptr IsParam(std::string param_name) {
            value_ptr val = NULL;
            std::string pn = "@" + param_name;
            for (size_t i = 0; i < func_cur->params->len; i ++ ) {
                value_ptr param = (value_ptr)func_cur->params->buffer[i];
                if (pn == param->name) {
                    value_ptr v_alloc = NewValue();
                    v_alloc->name = "%" + pn.substr(1);
                    v_alloc->ty.tag = param->ty.tag;
                    v_alloc->kind.tag = IR_ALLOC;
                    AddValue(v_alloc);
                    value_ptr v_store = NewValue();
                    v_store->kind.tag = IR_STORE;
                    v_store->kind.data.store.dest = v_alloc;
                    v_store->kind.data.store.value = param;
                    AddValue(v_store);
                    value_ptr v_load = NewValue();
                    v_load->ty.tag = v_alloc->ty.tag;
                    v_load->kind.tag = IR_LOAD;
                    v_load->kind.data.load.src = v_alloc;
                    AddValue(v_load);
                    val = v_load;
                    // val = param;
                    break;
                }
            }
            return val;
        }

        void AddGlobalValue(value_ptr val) {
            assert(false);
        }

        void AddValue(value_ptr val) {
            value_ptr last_val = NULL;
            if (bb_cur->insts->len > 0) 
                last_val = (value_ptr)bb_cur->insts->buffer[bb_cur->insts->len - 1];
            
            if ((last_val == NULL) 
             || ((last_val->kind.tag != IR_RETURN) 
             && (last_val->kind.tag != IR_JUMP) 
             && (last_val->kind.tag != IR_BRANCH))) {
                // 将该指令添加到当前的基本块中
                bb_cur->insts->buffer.push_back(val);
                bb_cur->insts->len ++;
                bb_cur->insts->kind = RISK_VALUE;
            }
        }

        void InsertValue(value_ptr val, basic_block_ptr bb) {
            value_ptr last_val = NULL;
            if (bb->insts->len > 0) 
                last_val = (value_ptr)bb->insts->buffer[bb->insts->len - 1];
            
            if ((last_val == NULL) 
             || ((last_val->kind.tag != IR_RETURN) 
             && (last_val->kind.tag != IR_JUMP) 
             && (last_val->kind.tag != IR_BRANCH))) {
                // 将该指令添加到当前的基本块中
                bb->insts->buffer.push_back(val);
                bb->insts->len ++;
                bb->insts->kind = RISK_VALUE;
            }
        }

        void InsertParam(value_ptr param, slice_ptr params) {
            params->buffer.push_back(param);
            params->len ++;
            params->kind = RISK_VALUE;
        }

        void AddBasicBlock(basic_block_ptr bb) {
            func_cur->bbs->buffer.push_back(bb);
            func_cur->bbs->len ++;
            func_cur->bbs->kind = RISK_BASIC_BLOCK;
        }
};

class Visitor {
    public:
        Translate_Helper *tr;
        virtual void *visit(CompUnitAST *) { return NULL; }
        virtual void *visit(CompItemsAST *) { return NULL; }
        virtual void *visit(CompItemAST *) { return NULL; }
        virtual void *visit(FuncDefAST *) { return NULL; }
        virtual void *visit(FuncTypeAST *) { return NULL; }
        virtual void *visit(FuncFParamsAST *) { return NULL; }
        virtual void *visit(FuncFParamAST *) { return NULL; }
        virtual void *visit(BlockAST *) { return NULL; }
        virtual void *visit(BlockItemsAST *) { return NULL; }
        virtual void *visit(BlockItemAST *) { return NULL; }
        virtual void *visit(DeclAST *) { return NULL; }
        virtual void *visit(ConstDeclAST *) { return NULL; }
        virtual void *visit(ConstDefsAST *) { return NULL; }
        virtual void *visit(ConstDefAST *) { return NULL; }
        virtual void *visit(ConstInitValAST *) { return NULL; }
        virtual void *visit(VarDeclAST *) { return NULL; }
        virtual void *visit(VarDefsAST *) { return NULL; }
        virtual void *visit(VarDefAST *) { return NULL; }
        virtual void *visit(InitValAST *) { return NULL; }
        virtual void *visit(StmtAST *) { return NULL; }
        virtual void *visit(OpenStmtAST *) { return NULL; }
        virtual void *visit(ClosedStmtAST *) { return NULL; }
        virtual void *visit(NonIfStmtAST *) { return NULL; }
        virtual void *visit(LessStmtAST *) { return NULL; }
        virtual void *visit(ConstExpAST *) { return NULL; }
        virtual void *visit(ExpAST *) { return NULL; }
        virtual void *visit(PrimaryExpAST *) { return NULL; }
        virtual void *visit(UnaryExpAST *) { return NULL; }
        virtual void *visit(FuncRParamsAST *) { return NULL; }
        virtual void *visit(MulExpAST *) { return NULL; }
        virtual void *visit(AddExpAST *) { return NULL; }
        virtual void *visit(RelExpAST *) { return NULL; }
        virtual void *visit(EqExpAST *) { return NULL; }
        virtual void *visit(LAndExpAST *) { return NULL; }
        virtual void *visit(LOrExpAST *) { return NULL; }

        Visitor() {
            tr = new Translate_Helper;
        }

        virtual ~Visitor() {
            delete tr;
            tr = NULL;
        }
};

#endif