#ifndef VISITOR_HPP
#define VISITOR_HPP

// #include "AST.hpp"
class BaseAST;
class CompUnitAST;
class FuncDefAST;
class FuncTypeAST;
class BlockAST;
class StmtAST;

class Visitor {
    public:
        virtual void visit(CompUnitAST *) {}
        virtual void visit(FuncDefAST *) {}
        virtual void visit(FuncTypeAST *) {}
        virtual void visit(BlockAST *) {}
        virtual void visit(StmtAST *) {}
};

#endif