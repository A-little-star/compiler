#ifndef AST_HPP
#define AST_HPP
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include "visitor.hpp"
#include "../midend/SymTable.hpp"
class BaseAST {
    public:
        virtual ~BaseAST() = default;

        virtual void Dump() const = 0;
        virtual void *accept(Visitor *v) = 0;
        virtual int get_value() { return 0; }
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
        std::unique_ptr<BaseAST> blockitems;

        void Dump() const override {
            std::cout << "Block {\n";
            blockitems->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class BlockItemsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> blockitems;

        void Dump() const override {
            for (size_t i = 0; i < blockitems.size(); i ++ ) {
                blockitems[i]->Dump();
            }
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class BlockItemAST : public BaseAST {
    public:
        enum type {DECL, STMT} type;
        std::unique_ptr<BaseAST> decl;
        std::unique_ptr<BaseAST> stmt;

        void Dump() const override {
            std::cout << "BlockItem: {\n";
            if (type == DECL) decl->Dump();
            else stmt->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class DeclAST : public BaseAST {
    public:
        enum {CON_DECL, VAR_DECL} type;
        std::unique_ptr<BaseAST> constdecl;
        std::unique_ptr<BaseAST> vardecl;

        void Dump() const override {
            std::cout << "DeclAST: {\n";
            if (type == CON_DECL) constdecl->Dump();
            else if (type == VAR_DECL) vardecl->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class ConstDeclAST : public BaseAST {
    public:
        std::string btype;
        std::unique_ptr<BaseAST> constdefs;

        void Dump() const override {
            std::cout << "ConstDeclAST: {\n";
            constdefs->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class ConstDefsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> constdefs;

        void Dump() const override {
            for (size_t i = 0; i < constdefs.size(); i ++ ) {
                constdefs[i]->Dump();
            }
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class ConstDefAST : public BaseAST {
    public:
        std::string ident;
        std::unique_ptr<BaseAST> constinitval;

        void Dump() const override {
            std::cout << "ConstDefAST: {\n";
            constinitval->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class ConstInitValAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> constexp;

        void Dump() const override {
            std::cout << "ConstInitValAST: {\n";
            constexp->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }

        int get_value() override {
            return constexp->get_value();
        }
};

class VarDeclAST : public BaseAST {
    public:
        std::string btype;
        std::unique_ptr<BaseAST> vardefs;

        void Dump() const override {
            std::cout << "VarDeclAST: {\n";
            vardefs->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class VarDefsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> vardefs;

        void Dump() const override {
            for (size_t i = 0; i < vardefs.size(); i ++ ) {
                vardefs[i]->Dump();
            }
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class VarDefAST : public BaseAST {
    public:
        enum {NO_VALUE, HAS_VALUE} type;
        std::string ident;
        std::unique_ptr<BaseAST> initval;

        void Dump() const override {
            std::cout << "VarDefAST: {\n";
            initval->Dump();
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class InitValAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> exp;

        void Dump() const override {
            std::cout << "InitValAST: {\n";
            exp->Dump();
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class StmtAST : public BaseAST {
    public:
        /*
        指令类型：
        assign 表示赋值指令
        return 表示return指令
        void 表示空语句（即只有表达式，没有进行赋值，包括";"）
        block 表示语句块
        */
        enum {ASSIGN, RETURN, VOID, BLOCK} type;
        std::string lval;
        std::string ret;
        std::unique_ptr<BaseAST> exp;
        std::unique_ptr<BaseAST> block;

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

class ConstExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> exp;

        void Dump() const override {
            std::cout << "ConstExpAST: {\n";
            exp->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }

        int get_value() override {
            return exp->get_value();
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

        int get_value() override {
            return lorexp->get_value();
        }
};

class PrimaryExpAST : public BaseAST {
    public:
        enum type {EXP, NUM, LVAL} type;
        std::unique_ptr<BaseAST> exp;
        std::string lval;
        int number;

        void Dump() const override {
            if (this->type == NUM) {
                std::cout << number << std::endl;
            }
            else if (type == EXP) {
                std::cout << "PrimaryExp {\n";
                exp->Dump();
                std::cout << "}\n";
            }
            else {
                std::cout << lval << std::endl;
            }
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }

        int get_value() override {
            if (type == EXP) return exp->get_value();
            else if (type == LVAL) return bt.current->symtable[lval].value;
            else return number;
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

        int get_value() override {
            if (type == NAN) return primaryexp->get_value();
            else {
                if (unaryop == "+") return unaryexp->get_value();
                else if (unaryop == "-") return -unaryexp->get_value();
                else if (unaryop == "!") return !unaryexp->get_value();
                else {
                    printf("There is a exception in UnaryExpAST!\n");
                    return -1;
                }
            }
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

        int get_value() override {
            if (type == NAN) return unaryexp->get_value();
            else if (mulop == "*") return mulexp->get_value() * unaryexp->get_value();
            else if (mulop == "/") return mulexp->get_value() / unaryexp->get_value();
            else if (mulop == "%") return mulexp->get_value() % unaryexp->get_value();
            else {
                printf("There is a exception in MulExpAST!\n");
                return -1;
            }
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

        int get_value() override {
            if (type == NAN) return mulexp->get_value();
            else if (addop == "+") return addexp->get_value() + mulexp->get_value();
            else if (addop == "-") return addexp->get_value() - mulexp->get_value();
            else 
            {
                printf("There is a exception in AddExpAST!\n");
                return -1;
            }
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

        int get_value() override {
            if (type == NAN) return addexp->get_value();
            else if (relop == "<") return relexp->get_value() < addexp->get_value();
            else if (relop == ">") return relexp->get_value() > addexp->get_value();
            else if (relop == "<=") return relexp->get_value() <= addexp->get_value();
            else if (relop == ">=") return relexp->get_value() >= addexp->get_value();
            else 
            {
                printf("There is a exception in RelExpAST!\n");
                return -1;
            }
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

        int get_value() override {
            if (type == NAN) return relexp->get_value();
            else if (eqop == "==") return eqexp->get_value() == relexp->get_value();
            else if (eqop == "!=") return eqexp->get_value() != relexp->get_value();
            else 
            {
                printf("There is a exception in EqExpAST!\n");
                return -1;
            }
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

        int get_value() override {
            if (type == NAN) return eqexp->get_value();
            else return landexp->get_value() && eqexp->get_value();
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

        int get_value() override {
            if (type == NAN) return landexp->get_value();
            else return lorexp->get_value() || landexp->get_value();
        }
};

#endif