#ifndef AST_H
#define AST_H
#include <iostream>
class BaseAST {
    public:
        virtual ~BaseAST() = default;

        virtual void Dump() const = 0;
};

class CompUnitAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_def;

        void Dump() const override {
            func_def->Dump();
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
};

class FuncTypeAST : public BaseAST {
    public:
        std::string type;

        void Dump() const override {
            if (type == "int")
                std::cout << "i32" << ' ';
        }
};

class BlockAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> stmt;

        void Dump() const override {
            std::cout << "{" << std::endl << "%entry:" << std::endl;
            stmt->Dump();
            std::cout << std::endl << "}";
        }
};

class StmtAST : public BaseAST {
    public:
        std::string ret;
        int number;

        void Dump() const override {
            std::cout << "  ret " << number;
        }
};

#endif