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
        virtual std::string get_name() { return ""; }
};

class CompUnitAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> compitems;

        void Dump() const override {
            std::cout << "CompUnit {\n";
            compitems->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class CompItemsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> compitems;
        std::vector<std::unique_ptr<BaseAST>> decls;
        std::vector<std::unique_ptr<BaseAST>> funcdefs;

        void Dump() const override {
            for (size_t i = 0; i < compitems.size(); i ++ ) {
                compitems[i]->Dump();
            }
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class CompItemAST : public BaseAST {
    public:
        enum {DECL, FUNC} type;
        std::unique_ptr<BaseAST> decl;
        std::unique_ptr<BaseAST> funcdef;

        void Dump() const override {
            std::cout << "CompItem: {\n";
            if (type == DECL) decl->Dump();
            else funcdef->Dump();
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class FuncDefAST : public BaseAST {
    public:
        enum {NO_PARAMS, HAS_PARAMS} type;
        // std::unique_ptr<BaseAST> func_type;
        std::string func_type;
        std::string ident;
        std::unique_ptr<BaseAST> block;
        std::unique_ptr<BaseAST> funcfparams;

        void Dump() const override {
            std::cout << "FuncDef {\n";
            // func_type->Dump();
            block->Dump();
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class FuncFParamsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> funcfparams;

        void Dump() const override {

        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class FuncFParamAST : public BaseAST {
    public:
        enum {INT, ARRAY} type;
        std::string btype;
        std::string ident;
        std::unique_ptr<BaseAST> constexps;

        void Dump() const override {

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
        enum {VALUE, ARRAY} d_type;
        std::string ident;
        // std::unique_ptr<BaseAST> constexp;
        std::unique_ptr<BaseAST> constexps;
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
        enum {VALUE, ARRAY} d_type;
        std::unique_ptr<BaseAST> constexp;
        // std::unique_ptr<BaseAST> constexps;
        std::unique_ptr<BaseAST> constinitvals;

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

class ConstInitValsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> constinitvals;

        void Dump() const override {

        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class ConstExpsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> constexps;

        void Dump() const override {

        }
        void *accept(Visitor *v) override {
            return v->visit(this);
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
        enum {VALUE, ARRAY} d_type;
        std::string ident;
        // std::unique_ptr<BaseAST> constexp;
        std::unique_ptr<BaseAST> constexps;
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
        enum {VALUE, ARRAY} d_type;
        std::unique_ptr<BaseAST> exp;
        // std::unique_ptr<BaseAST> exps;
        std::unique_ptr<BaseAST> initvals;

        void Dump() const override {
            std::cout << "InitValAST: {\n";
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

class InitValsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> initvals;

        void Dump() const override {

        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class ExpsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> exps;

        void Dump() const override {

        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class StmtAST : public BaseAST {
    public:
        enum {OPENSTMT, CLOSEDSTMT} type;
        std::unique_ptr<BaseAST> openstmt;
        std::unique_ptr<BaseAST> closedstmt;

        void Dump() const override {
            std::cout << "Stmt :{\n";
            if (type == OPENSTMT) openstmt->Dump();
            else if (type == CLOSEDSTMT) closedstmt->Dump();
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class OpenStmtAST : public BaseAST {
    public:
        enum {IF, IF_ELSE} type;
        std::unique_ptr<BaseAST> exp;
        std::unique_ptr<BaseAST> stmt;
        std::unique_ptr<BaseAST> closedstmt;
        std::unique_ptr<BaseAST> openstmt;

        void Dump() const override {
            std::cout << "OpenStmt: {\n";
            if (type == IF) {
                std::cout << "if {\n";
                exp->Dump();
                std::cout << "}\n{";
                stmt->Dump();
                std::cout << "}\n";
            }
            else if (type == IF_ELSE) {
                std::cout << "if {\n";
                exp->Dump();
                std::cout << "}\n{";
                closedstmt->Dump();
                std::cout << "}\nelse {";
                openstmt->Dump();
                std::cout << "}\n";
            }
            std::cout << "}\n";
        }

        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class ClosedStmtAST : public BaseAST {
    public:
        enum {NONIF, IF_ELSE} type;
        std::unique_ptr<BaseAST> nonifstmt;
        std::unique_ptr<BaseAST> exp;
        std::unique_ptr<BaseAST> closedstmt_if;
        std::unique_ptr<BaseAST> closedstmt_else;

        void Dump() const override {
            std::cout << "ClosedStmt: {\n";
            if (type == NONIF) {
                nonifstmt->Dump();
            }
            else {
                std::cout << "if {\n";
                exp->Dump();
                std::cout << "}\n{";
                closedstmt_if->Dump();
                std::cout << "}\nelse {";
                closedstmt_else->Dump();
                std::cout << "}\n";
            }
            std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class NonIfStmtAST : public BaseAST {
    public:
        enum {STMT, BLOCK, WHILE, FOR} type;
        std::unique_ptr<BaseAST> lessstmt;
        std::unique_ptr<BaseAST> block;
        std::unique_ptr<BaseAST> exp;
        std::unique_ptr<BaseAST> stmt;
        std::unique_ptr<BaseAST> blockitem;

        void Dump() const override {

        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
};

class LessStmtAST : public BaseAST {
    public:
        /*
        指令类型：
        assign 表示赋值指令
        return 表示return指令
        void 表示空语句（即只有表达式，没有进行赋值，包括";"）
        */
        enum {ASSIGN, RETURN, VOID, BREAK, CONTINUE} type;
        // std::string lval;
        std::unique_ptr<BaseAST> lval;
        std::string ret;
        std::unique_ptr<BaseAST> exp;
        std::unique_ptr<BaseAST> stmt;
        std::unique_ptr<BaseAST> stmt_end;
        std::unique_ptr<BaseAST> blockitem;

        void Dump() const override {
            std::cout << "NonIfStmt {\n";
            if (type == RETURN) {
                std::cout << "ret ";
                if (exp != NULL) exp->Dump();
                std::cout << "}\n";
            }
            else if (type == ASSIGN) {
                // std::cout << lval;
                lval->Dump();
                exp->Dump();
                std::cout << "}\n";
            }
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
        std::unique_ptr<BaseAST> lval;
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
                std::cout << "PrimaryExp {\n";
                lval->Dump();
                std::cout << "}\n";
            }
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }

        int get_value() override {
            if (type == EXP) return exp->get_value();
            else if (type == LVAL) return lval->get_value();
            else return number;
        }
};

class LValAST : public BaseAST {
    public:
        enum {VALUE, ARRAY} d_type;
        std::string ident;
        // std::unique_ptr<BaseAST> exp;
        std::unique_ptr<BaseAST> exps;

        void Dump() const override {
            // std::cout << "LVal: {\n";
            std::cout << ident;
            // std::cout << "}\n";
        }
        void *accept(Visitor *v) override {
            return v->visit(this);
        }
        int get_value() override {
            return bt.current->symtable[ident].value;
        }
        std::string get_name() override {
            return ident;
        }
};

class UnaryExpAST : public BaseAST {
    public:
        enum type {NAN, EXP, FUNC_NO_PARAMS, FUNC_HAS_PARAMS} type;
        std::unique_ptr<BaseAST> primaryexp;
        std::unique_ptr<BaseAST> unaryexp;
        std::string unaryop;
        std::string ident;
        std::unique_ptr<BaseAST> funcrparams;

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

class FuncRParamsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> funcrparams;

        void Dump() const override {

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