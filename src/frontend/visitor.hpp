#ifndef VISITOR_HPP
#define VISITOR_HPP

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

class Visitor {
    public:
        void* helper;
        enum type {INT, } cur_type;
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
};

#endif