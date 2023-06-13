#include <memory>
#include <cstring>
#include "xc.hpp"
#include "../frontend/AST.hpp"
// #include "visitor.hpp"

class Visitor;
extern prog_ptr prog;

class GenIR : public Visitor {
    void *visit(CompUnitAST *CompUnit) {
        prog = new program;
        prog->funcs = new slice;
        prog->funcs->len = 0;
        prog->values = new slice;
        prog->values->len = 0;
        func_ptr func = (func_ptr)CompUnit->func_def->accept(this);
        // 将该函数添加到program之中
        prog->funcs->buffer.push_back(func);
        prog->funcs->len ++;
        prog->funcs->kind = RISK_FUNCTION;
        return NULL;
        // return prog;
    }
    void *visit(FuncDefAST *FuncDef) {
        // 创建一个新的函数类型，并为其成员变量分配空间
        func_ptr func = new function;
        func->bbs = new slice;
        func->bbs->len = 0;
        func->params = new slice;
        func->params->len = 0;
        
        // 递归处理
        ret_val_kind *ret_type = (ret_val_kind*)FuncDef->func_type->accept(this);
        func->kind = *ret_type;
        basic_block_ptr bb = (basic_block_ptr)FuncDef->block->accept(this);
        // 将该基本块添加到所在的函数中
        func->bbs->buffer.push_back(bb);
        func->bbs->len ++;
        func->bbs->kind = RISK_BASIC_BLOCK;
        return func;
    }
    void *visit(FuncTypeAST *FuncType) {
        // func_ptr func = (func_ptr)prog->funcs->buffer[0];
        // func->kind = RET_INT;
        ret_val_kind *ret_type = new ret_val_kind;
        *ret_type = RET_INT;
        return ret_type;
    }
    void *visit(BlockAST *Block) {
        // 创建一个新的Basic block，并为其成员变量分配空间；
        basic_block_ptr bb = new basic_block;
        bb->params = new slice;
        bb->params->len = 0;
        bb->insts = new slice;
        bb->insts->len = 0;
        
        // 访问基本块下的指令
        value_ptr val = (value_ptr)Block->stmt->accept(this);
        // 将该指令添加到对应的基本块中
        bb->insts->buffer.push_back(val);
        bb->insts->len ++;
        bb->insts->kind = RISK_INSTRUCTION;
        return bb;
    }
    void *visit(StmtAST *Stmt) {
        // 创建一个新的value类型（这里表示指令），并为其成员变量分配空间
        value_ptr val = new value;
        val->used = new slice;
        val->used->len = 0;
        val->used_by = new slice;
        val->used_by->len = 0;
        
        val->kind.tag = IR_RETURN;
        // val->kind.data.ret.value = new value;
        // value_ptr r = val->kind.data.ret.value;
        // r->kind.tag = IR_INTEGER;
        // r->kind.data.integer.value = 0;
        // return val;

        value_ptr val_ret = (value_ptr)Stmt->exp->accept(this);
        val->kind.data.ret.value = val_ret;
        
        // value_ptr r = val->kind.data.ret.value;
        // r->kind.tag = IR_INTEGER;
        // r->kind.data.integer.value = Stmt->number;
        return val;
    }
    void *visit(ExpAST *Exp) {
        // value_ptr val = new value;
        // val->used = new slice;
        // val->used->len = 0;
        // val->used_by = new slice;
        // val->used_by->len = 0;

        value_ptr val_ret = (value_ptr)Exp->unaryexp->accept(this);
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
        else {
            printf("There is a exception in visit of PrimaryExp!\n");
            return NULL;
        }
    }
    void *visit(UnaryExpAST *UnaryExp) {
        if (UnaryExp->type == UnaryExpAST::NAN || UnaryExp->unaryop == "+") {
            value_ptr val = (value_ptr)UnaryExp->primaryexp->accept(this);
            return val;
        }
        else if (UnaryExp->type == UnaryExpAST::EXP && UnaryExp->unaryop != "+") {
            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;

            if (UnaryExp->unaryop == "-") {
                val->kind.tag = IR_BINARY;
                val->kind.data.binary.op = SUB;
                val->kind.data.binary.lhs = new value;
                val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
                val->kind.data.binary.lhs->kind.data.integer.value = 0;
                val->kind.data.binary.rhs = (value_ptr)UnaryExp->unaryexp->accept(this);
            }
            else if (UnaryExp->unaryop == "!") {
                val->kind.tag = IR_BINARY;
                val->kind.data.binary.op = EQ;
                val->kind.data.binary.lhs = new value;
                val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
                val->kind.data.binary.lhs->kind.data.integer.value = 0;
                val->kind.data.binary.rhs = (value_ptr)UnaryExp->unaryexp->accept(this);
            }
            return val;
        }
    }
    // void *visit(UnaryOpAST *UnaryOp) {

    // }
};