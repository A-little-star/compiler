#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <assert.h>
#include "../midend/xc.hpp"
#include <unordered_map>

extern std::unordered_map<value_ptr, value_ptr> param2value;
extern std::unordered_map<std::string, func_ptr> func_map;

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
class ConstInitValsAST;
class ConstExpAST;
class ConstExpsAST;
class VarDeclAST;
class VarDefsAST;
class VarDefAST;
class InitValAST;
class InitValsAST;

// 语句类型
class StmtAST;
class OpenStmtAST;
class ClosedStmtAST;
class NonIfStmtAST;
class LessStmtAST;

// 表达式类型
class ExpAST;
class ExpsAST;
class PrimaryExpAST;
class UnaryExpAST;
class UnaryOpAST;
class MulExpAST;
class AddExpAST;
class RelExpAST;
class EqExpAST;
class LAndExpAST;
class LOrExpAST;
class LValAST;

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
        // 当前变量声明是否处在全局作用域
        bool is_global;
        // 当前所处理的数组
        value_ptr arr_cur;
        // 当前处理的数组的下标
        int arr_index_cur[100];
        // 数组维度栈
        std::vector<type_kind*> arr;
        // 当前处理的数组长度
        int arr_len;
        // 当前处理的LVal名称
        std::string lval_name_cur;
        // 当前是否正在进行数组的初始化
        bool is_array_init;

        int GetDim(type_kind ty) {
            int dim = 0;
            while (ty.tag == KOOPA_TYPE_ARRAY) {
                dim ++;
                ty = *ty.data.array.base;
            }
            return dim;
        }

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

        func_ptr NewFunction() {
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

        void AddGlobalValue(value_ptr global_val) {
            prog_cur->values->buffer.push_back(global_val);
            prog_cur->values->len ++;
            prog_cur->values->kind = RISK_VALUE;
        }

        // 判断变量名为param_name的变量是否存在于当前函数的参数列表中，如果存在，返回该存数的value指针，否则返回NULL
        value_ptr IsParam(std::string param_name) {
            // printf("There is a param.\n");
            value_ptr val = NULL;
            std::string pn = "@" + param_name;
            for (size_t i = 0; i < func_cur->params->len; i ++ ) {
                value_ptr param = (value_ptr)func_cur->params->buffer[i];
                if (pn == param->name) {
                    if (!param2value.count(param)) {
                        value_ptr v_alloc = NewValue();
                        v_alloc->name = "%" + pn.substr(1);
                        v_alloc->ty.tag = KOOPA_TYPE_POINTER;
                        v_alloc->ty.data.pointer.base = new type_kind;
                        *v_alloc->ty.data.pointer.base = param->ty;
                        // v_alloc->ty.tag = param->ty.tag;
                        v_alloc->kind.tag = IR_ALLOC;
                        AddValue(v_alloc);
                        value_ptr v_store = NewValue();
                        v_store->kind.tag = IR_STORE;
                        v_store->kind.data.store.dest = v_alloc;
                        v_store->kind.data.store.value = param;
                        AddValue(v_store);
                        param2value[param] = v_alloc;
                    }
                    // if (param2value[param]->ty.data.pointer.base->tag == KOOPA_TYPE_INT32) {
                    //     val = param2value[param];
                    //     break;
                    // }
                    value_ptr v_load = NewValue();
                    v_load->ty = *param2value[param]->ty.data.pointer.base;
                    // v_load->ty.tag = param2value[param]->ty.tag;
                    v_load->kind.tag = IR_LOAD;
                    v_load->kind.data.load.src = param2value[param];
                    AddValue(v_load);
                    val = v_load;
                    break;
                }
            }
            return val;
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

        void AddLibFunc() {
            func_ptr getint = NewFunction();
            getint->name = "@getint";
            getint->ty.tag = KOOPA_TYPE_INT32;
            // AddFunc(getint);
            func_map["getint"] = getint;

            func_ptr getch = NewFunction();
            getch->name = "@getch";
            getch->ty.tag = KOOPA_TYPE_INT32;
            // AddFunc(getch);
            func_map["getch"] = getch;

            func_ptr getarray = NewFunction();
            getarray->name = "@getarray";
            getarray->ty.tag = KOOPA_TYPE_INT32;
            value_ptr getarray_p = NewValue();
            getarray_p->ty.tag = KOOPA_TYPE_POINTER;
            getarray_p->kind.tag = IR_FUNC_ARG;
            getarray_p->kind.data.func_arg.index = 1;
            InsertParam(getarray_p, getarray->params);
            // AddFunc(getarray);
            func_map["getarray"] = getarray;

            func_ptr putint = NewFunction();
            putint->name = "@putint";
            putint->ty.tag = KOOPA_TYPE_UNIT;
            value_ptr putint_p = NewValue();
            putint_p->ty.tag = KOOPA_TYPE_INT32;
            putint_p->kind.tag = IR_FUNC_ARG;
            putint_p->kind.data.func_arg.index = 1;
            InsertParam(putint_p, putint->params);
            func_map["putint"] = putint;

            func_ptr putch = NewFunction();
            putch->name = "@putch";
            putch->ty.tag = KOOPA_TYPE_UNIT;
            value_ptr putch_p = NewValue();
            putch_p->ty.tag = KOOPA_TYPE_INT32;
            putch_p->kind.tag = IR_FUNC_ARG;
            putch_p->kind.data.func_arg.index = 1;
            InsertParam(putch_p, putch->params);
            func_map["putch"] = putch;

            func_ptr putarray = NewFunction();
            putarray->name = "@putarray";
            putarray->ty.tag = KOOPA_TYPE_UNIT;
            value_ptr putarray_p1 = NewValue();
            putarray_p1->ty.tag = KOOPA_TYPE_INT32;
            putarray_p1->kind.tag = IR_FUNC_ARG;
            putarray_p1->kind.data.func_arg.index = 1;
            InsertParam(putarray_p1, putarray->params);
            value_ptr putarray_p2 = NewValue();
            putarray_p2->ty.tag = KOOPA_TYPE_POINTER;
            putarray_p2->kind.tag = IR_FUNC_ARG;
            putarray_p2->kind.data.func_arg.index = 2;
            InsertParam(putarray_p2, putarray->params);
            func_map["putarray"] = putarray;

            func_ptr starttime = NewFunction();
            starttime->ty.tag = KOOPA_TYPE_UNIT;
            starttime->name = "@starttime";
            func_map["starttime"] = starttime;

            func_ptr stoptime = NewFunction();
            stoptime->ty.tag = KOOPA_TYPE_UNIT;
            stoptime->name = "@stoptime";
            func_map["stoptime"] = stoptime;
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
        virtual void *visit(ConstInitValsAST *) { return NULL; }
        virtual void *visit(VarDeclAST *) { return NULL; }
        virtual void *visit(VarDefsAST *) { return NULL; }
        virtual void *visit(VarDefAST *) { return NULL; }
        virtual void *visit(InitValAST *) { return NULL; }
        virtual void *visit(InitValsAST *) { return NULL; }
        virtual void *visit(StmtAST *) { return NULL; }
        virtual void *visit(OpenStmtAST *) { return NULL; }
        virtual void *visit(ClosedStmtAST *) { return NULL; }
        virtual void *visit(NonIfStmtAST *) { return NULL; }
        virtual void *visit(LessStmtAST *) { return NULL; }
        virtual void *visit(ConstExpAST *) { return NULL; }
        virtual void *visit(ConstExpsAST *) { return NULL; }
        virtual void *visit(ExpAST *) { return NULL; }
        virtual void *visit(ExpsAST *) { return NULL; }
        virtual void *visit(PrimaryExpAST *) { return NULL; }
        virtual void *visit(LValAST *) { return NULL; }
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