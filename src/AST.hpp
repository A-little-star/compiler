#ifndef AST_H
#define AST_H
#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_map>
#include "SymTable.cpp"

#define EXP_STACK_SIZE 100

extern int val_id;

enum ExpType {EXP, NUM};
typedef enum ExpType ExpType;

class BaseAST {
    public:
        mutable ExpType exptype;
        virtual ~BaseAST() = default;

        virtual void Dump() const = 0;
};

class CompUnitAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_def;

        void Dump() const override {
            std::cout << "CompUnitAST { ";
            func_def->Dump();
            std::cout << " }";
        }
};

class FuncDefAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_type;
        std::string ident;
        std::unique_ptr<BaseAST> block;

        void Dump() const override {
            std::cout << "FuncDefAST { ";
            func_type->Dump();
            std::cout << ", " << ident << ", ";
            block->Dump();
            std::cout << " }";
        }
};

class FuncTypeAST : public BaseAST {
    public:
        std::string type;

        void Dump() const override {
            std::cout << "FuncTypeAST { ";
            std::cout << type;
            std::cout << " }";
        }
};

class BlockTypeAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> stmt;

        void Dump() const override {
            std::cout << "BlockAST { ";
            stmt->Dump();
            std::cout << " }";
        }
};

class StmtAST : public BaseAST {
    public:
        std::string ret;
        int number;

        void Dump() const override {
            std::cout << "StmtAST { " << "return" << number << std::endl;
        }
};

#endif