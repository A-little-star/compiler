#ifndef AST_H
#define AST_H
#include <iostream>
#include <cstring>
class BaseAST {
    public:
        virtual ~BaseAST() = default;

        virtual std::string Dump() const = 0;
};

class CompUnitAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_def;

        std::string Dump() const override {
            return func_def->Dump();
        }
};

class FuncDefAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_type;
        std::string ident;
        std::unique_ptr<BaseAST> block;

        std::string Dump() const override {
            std::string res = "fun @";
            res += ident;
            res += "(): ";
            res += func_type->Dump();
            res += block->Dump();
            return res;
            std::cout << "fun @" << ident << "(): ";
            func_type->Dump();
            block->Dump();
        }
};

class FuncTypeAST : public BaseAST {
    public:
        std::string type;

        std::string Dump() const override {
            if (type == "int")
                return "i32";
        }
};

class BlockAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> stmt;

        std::string Dump() const override {
            std::string res = "{\n%entry:\n";
            res += stmt->Dump();
            res += "\n}";
            return res;
            std::cout << "{" << std::endl << "%entry:" << std::endl;
            stmt->Dump();
            std::cout << std::endl << "}";
        }
};

class StmtAST : public BaseAST {
    public:
        std::string ret;
        int number;

        std::string Dump() const override {
            std::string res = "  ret ";
            res += std::to_string(number);
            return res;
            std::cout << "  ret " << number;
        }
};

#endif