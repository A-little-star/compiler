#ifndef AST_HPP
#define AST_HPP
#include <iostream>
#include <memory>
#include "visitor.hpp"
class BaseAST {
    public:
        virtual ~BaseAST() = default;

        virtual void Dump() const = 0;
        virtual void accept(Visitor *v) = 0;
};

class CompUnitAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_def;

        void Dump() const override {
            func_def->Dump();
        }

        void accept(Visitor *v) override {
            v->visit(this);
        }
};

class FuncDefAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_type;
        std::string ident;
        std::unique_ptr<BaseAST> block;

        void Dump() const override {
            std::cout << "fun @" << ident << "(): ";
            func_type->Dump();
            block->Dump();
        }

        void accept(Visitor *v) override {
            v->visit(this);
        }
};

class FuncTypeAST : public BaseAST {
    public:
        std::string type;

        void Dump() const override {
            if (type == "int")
                std::cout << "i32" << ' ';
        }

        void accept(Visitor *v) override {
            v->visit(this);
        }
};

class BlockAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> stmt;

        void Dump() const override {
            std::cout << "{" << std::endl << "%entry:" << std::endl;
            stmt->Dump();
            std::cout << std::endl << "}" << std::endl;
        }

        void accept(Visitor *v) override {
            v->visit(this);
        }
};

class StmtAST : public BaseAST {
    public:
        std::string ret;
        int number;

        void Dump() const override {
            std::cout << "  ret " << number;
        }

        void accept(Visitor *v) override {
            v->visit(this);
        }
};

#endif