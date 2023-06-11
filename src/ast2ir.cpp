#include "xc.hpp"
#include "AST.hpp"
#include "visitor.hpp"

prog_ptr prog;

class GenIR : public Visitor {
    void visit(AST::CompUnitAST *CompUnit) {
        CompUnit->func_def->accept(this);
    }
    void visit(AST::FuncDefAST *FuncDef) {
        func_ptr func = (func_ptr)malloc(sizeof(func_ptr));
        prog->funcs->buffer.push_back(func);
        prog->funcs->len ++;
        prog->funcs->kind = RISK_FUNCTION;
        FuncDef->name = "main";
        FuncDef->func_type->accept(this);
        FuncDef->block->accept(this);
    }
    void visit(AST::FuncTypeAST *FuncType) {
        func_ptr func = (func_ptr)prog->funcs->buffer[0];
        func->kind = INT;
    }
    void visit(AST::BlockAST *Block) {
        block_ptr bb = (block_ptr)malloc(sizeof(block_ptr));
        func_ptr func = (func_ptr)prog->funcs->buffer[0];
        func->bbs->buffer.push_back(bb);
        func->bbs->len ++;
        func->bbs->kind = RISK_BASIC_BLOCK;
        Block->blockitems->accept(this);
    }
    void visit(AST::BlockItemsAST *BlockItems) {
        func_ptr func = (func_ptr)prog->funcs->buffer[0];
        block_ptr bb = (block_ptr)func->bbs->buffer[0];
        for (int i = 0; i < BlockItems->blockitem.size(); i ++ )
            BlockItems->blockitem[i]->accept(this);
        
    }
    // void visit(AST::Block)
};