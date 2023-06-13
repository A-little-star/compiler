#ifndef AST_HPP
#define AST_HPP
#include <iostream>
#include <memory>
#include "visitor.hpp"
class BaseAST {
    public:
        virtual ~BaseAST() = default;

        virtual void Dump() const = 0;
        virtual void *accept(Visitor *v) = 0;
};

class CompUnitAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_def;

        void Dump() const override {
            func_def->Dump();
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
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

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class FuncTypeAST : public BaseAST {
    public:
        std::string type;

        void Dump() const override {
            if (type == "int")
                std::cout << "i32" << ' ';
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
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

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class StmtAST : public BaseAST {
    public:
        std::string ret;
        std::unique_ptr<BaseAST> exp;

        void Dump() const override {
            // std::cout << "  ret " << number;
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class ExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> unaryexp;

        void Dump() const override {
            
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class PrimaryExpAST : public BaseAST {
    public:
        enum type {EXP, NUM} type;
        std::unique_ptr<BaseAST> exp;
        int number;

        void Dump() const override {

        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class UnaryExpAST : public BaseAST {
    public:
        enum type {NAN, EXP} type;
        std::unique_ptr<BaseAST> primaryexp;
        std::unique_ptr<BaseAST> unaryexp;
        std::string unaryop;

        void Dump() const override {

        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

// class UnaryOpAST : public BaseAST {
//     public:
//         std::string opcode;

//         void Dump() const override {

//         }
//         void *accept(Visitor *v) override {
//             return v->visit(this);
//         }
// };

#endif