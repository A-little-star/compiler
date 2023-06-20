#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "../midend/xc.hpp"
// #include "AST.hpp"
class BaseAST;
class CompUnitAST;
class FuncDefAST;
class FuncTypeAST;
class BlockAST;
class StmtAST;
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
class BlockItemsAST;
class BlockItemAST;
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


class Translate_Helper {
    public:
        // 当前所在的基本块（Koopa IR中的概念）
        basic_block_ptr bb_cur;
        // 当前应该处理的类型
        type_kind ty_cur;

        value_ptr NewValue() {
            // 新建一个value类型，并为其成员变量分配内存空间
            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;
            return val;
        }

        void AddValue(value_ptr val) {
            // 将该指令添加到当前的基本块中
            bb_cur->insts->buffer.push_back(val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;
        }
};

class Visitor {
    public:
        Translate_Helper *tr;
        virtual void *visit(CompUnitAST *) { return NULL; }
        virtual void *visit(FuncDefAST *) { return NULL; }
        virtual void *visit(FuncTypeAST *) { return NULL; }
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
        virtual void *visit(ConstExpAST *) { return NULL; }
        virtual void *visit(ExpAST *) { return NULL; }
        virtual void *visit(PrimaryExpAST *) { return NULL; }
        virtual void *visit(UnaryExpAST *) { return NULL; }
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