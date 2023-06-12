#include "xc.hpp"
#include "AST.hpp"
// #include "visitor.hpp"

class Visitor;
extern prog_ptr prog;

class GenIR : public Visitor {
    void visit(CompUnitAST *CompUnit) {
        printf("here is ok!\n");
        CompUnit->func_def->accept(this);
    }
    void visit(FuncDefAST *FuncDef) {
        printf("FuncDefAST is OK!\n");
        prog = (prog_ptr)malloc(sizeof(prog_ptr));
        func_ptr func = (func_ptr)malloc(sizeof(func_ptr));
        prog->funcs = (slice_ptr)malloc(sizeof(slice_ptr));
        prog->funcs->buffer.push_back(func);
        prog->funcs->len = 0;
        prog->funcs->len ++;
        prog->funcs->kind = RISK_FUNCTION;
        printf("Attention here!\n");

        // FuncDef->name = "main";
        FuncDef->func_type->accept(this);
        FuncDef->block->accept(this);
    }
    void visit(FuncTypeAST *FuncType) {
        printf("FuncTypeAST is OK!\n");
        func_ptr func = (func_ptr)prog->funcs->buffer[0];
        func->kind = RET_INT;
    }
    void visit(BlockAST *Block) {
        basic_block_ptr bb = (basic_block_ptr)malloc(sizeof(basic_block_ptr));
        func_ptr func = (func_ptr)prog->funcs->buffer[0];
        func->bbs = (slice_ptr)malloc(sizeof(slice_ptr));
        func->bbs->buffer.push_back(bb);
        func->bbs->len = 0;
        func->bbs->len ++;
        func->bbs->kind = RISK_BASIC_BLOCK;
        // Block->blockitems->accept(this);
        Block->stmt->accept(this);
    }
    void visit(StmtAST *Stmt) {
        printf("StmtAST is OK!\n");
        func_ptr func = (func_ptr)prog->funcs->buffer[0];
        basic_block_ptr bb = (basic_block_ptr)func->bbs->buffer[0];
        value_ptr val = (value_ptr)malloc(sizeof(value_ptr));
        bb->insts = (slice_ptr)malloc(sizeof(slice_ptr));
        bb->insts->buffer.push_back(val);
        bb->insts->len = 0;
        bb->insts->len ++;
        bb->insts->kind = RISK_INSTRUCTION;
        printf("bb is OK!\n");
        val->kind.tag = IR_RETURN;
        val->kind.data.ret.value = (value_ptr)malloc(sizeof(value_ptr));
        value_ptr r = val->kind.data.ret.value;
        r->kind.tag = IR_INTEGER;
        r->kind.data.integer.value = Stmt->number;
    }
};