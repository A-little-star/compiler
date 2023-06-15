#include <memory>
#include <cstring>
#include "xc.hpp"
#include "../frontend/AST.hpp"
#include "SymTable.hpp"
// #include "visitor.hpp"

class Visitor;
// extern prog_ptr prog;

class GenIR : public Visitor {
    void *visit(CompUnitAST *CompUnit) {
        // 创建一个新的program类型，并为其成员变量分配内存空间
        prog_ptr prog = new program;
        prog->funcs = new slice;
        prog->funcs->len = 0;
        prog->values = new slice;
        prog->values->len = 0;
        // 递归调用program下的funcdef
        func_ptr func = (func_ptr)CompUnit->func_def->accept(this);
        // 将该函数添加到program之中
        prog->funcs->buffer.push_back(func);
        prog->funcs->len ++;
        prog->funcs->kind = RISK_FUNCTION;
        return prog;
    }
    void *visit(FuncDefAST *FuncDef) {
        // 创建一个新的函数类型，并为其成员变量分配空间
        func_ptr func = new function;
        func->bbs = new slice;
        func->bbs->len = 0;
        func->params = new slice;
        func->params->len = 0;
        
        // 函数名
        func->name = FuncDef->ident;
        // 递归处理函数的返回值类型
        ret_val_kind *ret_type = (ret_val_kind*)FuncDef->func_type->accept(this);
        func->kind = *ret_type;
        // 递归处理函数的基本块
        basic_block_ptr bb = (basic_block_ptr)FuncDef->block->accept(this);
        // 将该基本块添加到所在的函数中
        func->bbs->buffer.push_back(bb);
        func->bbs->len ++;
        func->bbs->kind = RISK_BASIC_BLOCK;
        return func;
    }
    void *visit(FuncTypeAST *FuncType) {
        // 新建一个函数返回值类型的指针
        ret_val_kind *ret_type = new ret_val_kind;
        // 目前而言返回值均为int
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
        // helper 指针指向当前所在的基本块
        helper = (void*)bb;
        // 访问基本块下的指令，指令将在处理过程中逐渐push到bb的buffer中
        Block->blockitems->accept(this);
        return bb;
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
        Decl->constdecl->accept(this);
        return NULL;
    }
    void *visit(ConstDeclAST *ConstDecl) {
        if (ConstDecl->btype == "int") cur_type = Visitor::INT;
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
        int val = ConstDef->constinitval->get_value();
        symtable[ConstDef->ident] = {"i32", val, CON};
        return NULL;
    }
    void *visit(StmtAST *Stmt) {
        // 创建一个新的value类型（这里表示指令），并为其成员变量分配空间
        value_ptr val = new value;
        val->used = new slice;
        val->used->len = 0;
        val->used_by = new slice;
        val->used_by->len = 0;
        
        // 目前只处理return指令
        val->kind.tag = IR_RETURN;

        value_ptr val_ret = (value_ptr)Stmt->exp->accept(this);
        val->kind.data.ret.value = val_ret;
        // 将该指令添加到当前基本块中
        basic_block_ptr bb_cur = (basic_block_ptr)helper;
        bb_cur->insts->buffer.push_back(val);
        bb_cur->insts->len ++;
        bb_cur->insts->kind = RISK_VALUE;

        return val;
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
            value_ptr val = new value;
            val->kind.tag = IR_INTEGER;
            val->kind.data.integer.value = symtable[PrimaryExp->lval].value;
            return val;
        }
        else {
            printf("There is a exception in visit of PrimaryExp!\n");
            return NULL;
        }
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
            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;

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
            basic_block_ptr bb_cur = (basic_block_ptr)helper;
            bb_cur->insts->buffer.push_back(val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;
            
            return val;
        }
        else {
            printf("There is a exception in visit of UnaryExp!\n");
            return NULL;
        }
    }
    void *visit(MulExpAST *MulExp) {
        if (MulExp->type == MulExpAST::NAN) {
            value_ptr val = (value_ptr)MulExp->unaryexp->accept(this);
            return val;
        }
        else {
            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;

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
            basic_block_ptr bb_cur = (basic_block_ptr)helper;
            bb_cur->insts->buffer.push_back(val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;
            return val;
        }
    }
    void *visit(AddExpAST *AddExp) {
        if (AddExp->type == AddExpAST::NAN) {
            value_ptr val = (value_ptr)AddExp->mulexp->accept(this);
            return val;
        }
        else {
            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;

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
            basic_block_ptr bb_cur = (basic_block_ptr)helper;
            bb_cur->insts->buffer.push_back(val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;
            return val;
        }
    }
    void *visit(RelExpAST *RelExp) {
        if (RelExp->type == RelExpAST::NAN) {
            value_ptr val = (value_ptr)RelExp->addexp->accept(this);
            return val;
        }
        else {
            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;

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
            basic_block_ptr bb_cur = (basic_block_ptr)helper;
            bb_cur->insts->buffer.push_back(val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;
            return val;
        }
    }
    void *visit(EqExpAST *EqExp) {
        if (EqExp->type == EqExpAST::NAN) {
            value_ptr val = (value_ptr)EqExp->relexp->accept(this);
            return val;
        }
        else {
            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;

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
            basic_block_ptr bb_cur = (basic_block_ptr)helper;
            bb_cur->insts->buffer.push_back(val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;
            return val;
        }
    }
    void *visit(LAndExpAST *LAndExp) {
        if (LAndExp->type == LAndExpAST::NAN) {
            value_ptr val = (value_ptr)LAndExp->eqexp->accept(this);
            return val;
        }
        else {
            value_ptr l_val = new value;
            l_val->used = new slice;
            l_val->used->len = 0;
            l_val->used_by = new slice;
            l_val->used_by->len = 0;
            l_val->kind.tag = IR_BINARY;
            l_val->kind.data.binary.op = NOT_EQ;
            l_val->kind.data.binary.lhs = new value;
            l_val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
            l_val->kind.data.binary.lhs->kind.data.integer.value = 0;
            l_val->kind.data.binary.rhs = (value_ptr)LAndExp->landexp->accept(this);

            // 将该指令添加到当前的基本块中
            basic_block_ptr bb_cur = (basic_block_ptr)helper;
            bb_cur->insts->buffer.push_back(l_val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;

            value_ptr r_val = new value;
            r_val->used = new slice;
            r_val->used->len = 0;
            r_val->used_by = new slice;
            r_val->used_by->len = 0;
            r_val->kind.tag = IR_BINARY;
            r_val->kind.data.binary.op = NOT_EQ;
            r_val->kind.data.binary.lhs = new value;
            r_val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
            r_val->kind.data.binary.lhs->kind.data.integer.value = 0;
            r_val->kind.data.binary.rhs = (value_ptr)LAndExp->eqexp->accept(this);

            bb_cur->insts->buffer.push_back(r_val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;

            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;

            val->kind.tag = IR_BINARY;

            val->kind.data.binary.op = AND;

            val->kind.data.binary.lhs = l_val;
            val->kind.data.binary.rhs = r_val;
            // val->kind.data.binary.lhs = (value_ptr)LAndExp->landexp->accept(this);
            // val->kind.data.binary.rhs = (value_ptr)LAndExp->eqexp->accept(this);

            // 将该指令添加到当前的基本块中
            // basic_block_ptr bb_cur = (basic_block_ptr)helper;
            bb_cur->insts->buffer.push_back(val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;
            return val;
        }
    }
    void *visit(LOrExpAST *LOrExp) {
        if (LOrExp->type == LOrExpAST::NAN) {
            value_ptr val = (value_ptr)LOrExp->landexp->accept(this);
            return val;
        }
        else {
            value_ptr l_val = new value;
            l_val->used = new slice;
            l_val->used->len = 0;
            l_val->used_by = new slice;
            l_val->used_by->len = 0;
            l_val->kind.tag = IR_BINARY;
            l_val->kind.data.binary.op = NOT_EQ;
            l_val->kind.data.binary.lhs = new value;
            l_val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
            l_val->kind.data.binary.lhs->kind.data.integer.value = 0;
            l_val->kind.data.binary.rhs = (value_ptr)LOrExp->lorexp->accept(this);

            // 将该指令添加到当前的基本块中
            basic_block_ptr bb_cur = (basic_block_ptr)helper;
            bb_cur->insts->buffer.push_back(l_val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;

            value_ptr r_val = new value;
            r_val->used = new slice;
            r_val->used->len = 0;
            r_val->used_by = new slice;
            r_val->used_by->len = 0;
            r_val->kind.tag = IR_BINARY;
            r_val->kind.data.binary.op = NOT_EQ;
            r_val->kind.data.binary.lhs = new value;
            r_val->kind.data.binary.lhs->kind.tag = IR_INTEGER;
            r_val->kind.data.binary.lhs->kind.data.integer.value = 0;
            r_val->kind.data.binary.rhs = (value_ptr)LOrExp->landexp->accept(this);

            bb_cur->insts->buffer.push_back(r_val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;

            value_ptr val = new value;
            val->used = new slice;
            val->used->len = 0;
            val->used_by = new slice;
            val->used_by->len = 0;

            val->kind.tag = IR_BINARY;

            val->kind.data.binary.op = OR;

            val->kind.data.binary.lhs = l_val;
            val->kind.data.binary.rhs = r_val;
            // val->kind.data.binary.lhs = (value_ptr)LOrExp->lorexp->accept(this);
            // val->kind.data.binary.rhs = (value_ptr)LOrExp->landexp->accept(this);

            // 将该指令添加到当前的基本块中
            // basic_block_ptr bb_cur = (basic_block_ptr)helper;
            bb_cur->insts->buffer.push_back(val);
            bb_cur->insts->len ++;
            bb_cur->insts->kind = RISK_VALUE;
            return val;
        }
    }
};