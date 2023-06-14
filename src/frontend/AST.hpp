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
            std::cout << "CompUnit {\n";
            func_def->Dump();
            std::cout << "}\n";
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
            std::cout << "FuncDef {\n";
            func_type->Dump();
            block->Dump();
            std::cout << "}\n";
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
                std::cout << "FuncType" << ' ';
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class BlockAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> stmt;

        void Dump() const override {
            std::cout << "Block {\n";
            stmt->Dump();
            std::cout << "}\n";
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
            std::cout << "Stmt {\n";
            std::cout << "ret ";
            exp->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class ExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> lorexp;

        void Dump() const override {
            std::cout << "Exp {\n";
            lorexp->Dump();
            std::cout << "}\n";
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
            if (this->type == NUM) {
                std::cout << number << std::endl;
            }
            else {
                std::cout << "UnaryExp {\n";
                exp->Dump();
                std::cout << "}\n";
            }
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
            std::cout << "UnaryExp {\n";
            if (this->type == NAN) primaryexp->Dump();
            else {
                std::cout << "unaryop:" << unaryop << std::endl;;
                unaryexp->Dump();
            }
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class MulExpAST : public BaseAST {
    public:
        enum type {NAN, EXP} type;
        std::unique_ptr<BaseAST> unaryexp;
        std::unique_ptr<BaseAST> mulexp;
        std::string mulop;

        void Dump() const override {
            std::cout << "MulExp {\n";
            if (this->type == NAN) unaryexp->Dump();
            else {
                std::cout << "mulop:" << mulop << std::endl;
                mulexp->Dump();
                unaryexp->Dump();
            }
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class AddExpAST : public BaseAST {
    public:
        enum type {NAN, EXP} type;
        std::unique_ptr<BaseAST> mulexp;
        std::unique_ptr<BaseAST> addexp;
        std::string addop;

        void Dump() const override {
            std::cout << "AddExp {\n";
            if (this->type == NAN) mulexp->Dump();
            else {
                std::cout << "addop:" << addop << std::endl;
                addexp->Dump();
                mulexp->Dump();
            }
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class RelExpAST : public BaseAST {
    public:
        enum type {NAN, EXP} type;
        std::unique_ptr<BaseAST> addexp;
        std::unique_ptr<BaseAST> relexp;
        std::string relop;

        void Dump()  const override {
            std::cout << "RelExp {\n";
            if (type == NAN) addexp->Dump();
            else {
                std::cout << "relop:" << relop << std::endl;
                relexp->Dump();
                addexp->Dump();
            }
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class EqExpAST : public BaseAST {
    public:
        enum type {NAN, EXP} type;
        std::unique_ptr<BaseAST> relexp;
        std::unique_ptr<BaseAST> eqexp;
        std::string eqop;

        void Dump() const override {
            std::cout << "EqExp {\n";
            if (type == NAN) relexp->Dump();
            else {
                std::cout << "eqop:" << eqop << std::endl;
                relexp->Dump();
                eqexp->Dump();
            }
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class LAndExpAST : public BaseAST {
    public:
        enum type {NAN, EXP} type;
        std::unique_ptr<BaseAST> eqexp;
        std::unique_ptr<BaseAST> landexp;
        std::string landop;

        void Dump() const override {
            std::cout << "LAndExp {\n";
            if (type == NAN) eqexp->Dump();
            else {
                std::cout << "landop:" << landop << std::endl;
                landexp->Dump();
                eqexp->Dump();
            }
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class LOrExpAST : public BaseAST {
    public:
        enum type {NAN, EXP} type;
        std::unique_ptr<BaseAST> landexp;
        std::unique_ptr<BaseAST> lorexp;
        std::string lorop;

        void Dump() const override {
            std::cout << "LOrExp {\n";
            if (type == NAN) landexp->Dump();
            else {
                std::cout << "lorop:" << lorop << std::endl;
                lorexp->Dump();
                landexp->Dump();
            }
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

#endif