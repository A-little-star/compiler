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

class Visitor {
    public:
        void* helper;
        virtual void *visit(CompUnitAST *) { return NULL; }
        virtual void *visit(FuncDefAST *) { return NULL; }
        virtual void *visit(FuncTypeAST *) { return NULL; }
        virtual void *visit(BlockAST *) { return NULL; }
        virtual void *visit(StmtAST *) { return NULL; }
        virtual void *visit(ExpAST *) { return NULL; }
        virtual void *visit(PrimaryExpAST *) { return NULL; }
        virtual void *visit(UnaryExpAST *) { return NULL; }
        // virtual void *visit(UnaryOpAST *) { return NULL; }
};

#endif