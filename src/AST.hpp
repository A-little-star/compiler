#ifndef AST_H
#define AST_H
#include <iostream>
#include <cstring>
#include <vector>
#include <SymTable.hpp>

#define EXP_STACK_SIZE 100

extern int val_id;
extern int max_id;

enum ExpType {EXP, NUM};
typedef enum ExpType ExpType;

class BaseAST {
    public:
        mutable ExpType exptype;
        virtual ~BaseAST() = default;

        virtual std::string Dump() const = 0;
        virtual int Val() const = 0;
};

class CompUnitAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_def;

        std::string Dump() const override {
            return func_def->Dump();
        }
        int Val() const override {
            return 0;
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
        int Val() const override {
            return 0;
        }
};

class FuncTypeAST : public BaseAST {
    public:
        std::string type;

        std::string Dump() const override {
            if (type == "int")
                return "i32";
            else return "non-type";
        }
        int Val() const override {
            return 0;
        }
};

class BlockAST : public BaseAST {
    public:
        // std::vector<std::unique_ptr<BaseAST>> blockitems;
        std::unique_ptr<BaseAST> blockitems;

        std::string Dump() const override {
            std::string res = "{\n%entry:\n";
            res += blockitems->Dump();
            // for (int i = 0; i < (int)blockitems.size(); i ++ )
            //     res += blockitems[i]->Dump();
            res += "\n}";
            return res;
        }
        int Val() const override {
            return 0;
        }
};

class BlockItemsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> blockitem;

        std::string Dump() const override {
            std::string res;
            for (int i = 0; i < blockitem.size(); i ++ )
            {
                res += blockitem[i]->Dump();
            }
            return res;
        }
        int Val() const override {
            return 0;
        }
};

class BlockItemAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> decl;
        std::unique_ptr<BaseAST> stmt;

        std::string Dump() const override {
            std::string res;
            if (stmt)
            {
                res += stmt->Dump();
                return res;
            }
            else 
            {
                res += decl->Dump();
                return res;
            }
        }
        int Val() const override {
            return 0;
        }
};

class DeclAST :public BaseAST {
    public:
        std::unique_ptr<BaseAST> constdecl;
        std::unique_ptr<BaseAST> vardecl;

        std::string Dump() const override {
            std::string res;
            if (!constdecl)
            {
                res = vardecl->Dump();
            }
            else if (!vardecl)
            {
                res = constdecl->Dump();
            }
            return res;
        }
        int Val() const override {
            return 0;
        }
};

class ConstDeclAST : public BaseAST {
    public:
        std::string btype;
        std::unique_ptr<BaseAST> constdefs;

        std::string Dump() const override {
            std::string res;
            res += constdefs->Dump();
            // for (int i = 0; i < (int)constdefs.size(); i ++ )
            // {
            //     res += constdefs[i]->Dump();
            // }
            return res;
        }
        int Val() const override {
            return 0;
        }
};

class ConstDefsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> constdefs;

        std::string Dump() const override {
            std::string res;
            for (int i = 0; i < constdefs.size(); i ++ )
                res += constdefs[i]->Dump();
            return res;
        }
        int Val() const override {
            return 0;
        }
};

class ConstDefAST : public BaseAST {
    public:
        std::string ident;
        std::unique_ptr<BaseAST> constinitval;

        std::string Dump() const override {
            symtable[ident] = {"i32", constinitval->Val(), CON};
            return "";
        }
        int Val() const override {
            return 0;
        }
};

class ConstInitValAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> constexp;

        std::string Dump() const override {
            return "";
        }
        int Val() const override {
            return constexp->Val();
        }
};

class VarDeclAST: public BaseAST {
    public:
        std::string btype;
        std::unique_ptr<BaseAST> vardefs;

        std::string Dump() const override {
            std::string res;
            res += vardefs->Dump();
            return res;
        }
        int Val() const override {
            return 0;
        }
};

class VarDefsAST : public BaseAST {
    public:
        std::vector<std::unique_ptr<BaseAST>> vardefs;

        std::string Dump() const override {
            std::string res;
            for (int i = 0; i < vardefs.size(); i ++ )
                res += vardefs[i]->Dump();
            return res;
        }
        int Val() const override {
            return 0;
        }
};

class VarDefAST : public BaseAST {
    public:
        std::string ident;
        std::unique_ptr<BaseAST> initval;

        std::string Dump() const override {
            // initval->Dump();
            std::string res;
            if (initval)
            {
                std::string s = initval->Dump();
                // res += std::to_string(initval->Val());
                if (initval->exptype == NUM)
                {
                    res = "  @";
                    res += ident;
                    res += " = alloc i32\n  store ";
                    res += std::to_string(initval->Val());
                }
                else if (initval->exptype == EXP)
                {
                    res += s;
                    res += "  @";
                    res += ident;
                    res += " = alloc i32\n  store ";
                    res += "%";
                    res += std::to_string(val_id - 1);
                }
                res += ", @";
                res += ident;
                res += "\n";
                symtable[ident] = {"i32", initval->Val(), VAR};
            }
            else 
            {
                res += "  @";
                res += ident;
                res += " = alloc i32\n";
                symtable[ident] = {"i32", 0, VAR};
            }
            return res;
        }
        int Val() const override {
            return 0;
        }
};

class InitValAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> exp;

        std::string Dump() const override {
            std::string res;
            res += exp->Dump();
            exptype = exp->exptype;
            return res;
        }
        int Val() const override {
            return exp->Val();
        }
};

class ConstExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> exp;

        std::string Dump() const override {
            return "";
        }

        int Val() const override {
            return exp->Val();
        }
};



class StmtAST : public BaseAST {
    public:
        std::string ret;
        std::unique_ptr<BaseAST> exp;
        std::string lval;

        std::string Dump() const override {
            std::string res;
            std::string s = exp->Dump();

            if (lval == "")
            {
                if (exp->exptype == EXP)
                {
                    res += s;
                    res += "  ret";
                    res += " %";
                    res += std::to_string(val_id - 1);
                }
                else
                {
                    res += "  ret ";
                    res += s;
                }
            }
            else 
            {
                if (exp->exptype == EXP)
                {
                    res += s;
                    res += "  store %";
                    res += std::to_string(val_id - 1);
                    res += ", @";
                    res += lval;
                    res += "\n";
                    symtable[lval].value = exp->Val();
                }
                else 
                {
                    res += "  store ";
                    res += s;
                    res += ", @";
                    res += lval;
                    res += "\n";
                    symtable[lval].value = exp->Val();
                }
            }
            return res;
        }
        int Val() const override {
            return 0;
        }
};

class ExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> lorexp;

        std::string Dump() const override {
            std::string res = lorexp->Dump();
            exptype = lorexp->exptype;
            return res;
        }
        int Val() const override {
            return lorexp->Val();
        }
};

class PrimaryExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> exp;
        std::string lval;
        int number;

        std::string Dump() const override {
            std::string res;
            if (exp)
            {
                res += exp->Dump();
                exptype = EXP;
            }
            else if (lval != "")
            {
                if (symtable[lval].kind == CON)
                {
                    exptype = NUM;
                    res += std::to_string(symtable[lval].value);
                }
                else if (symtable[lval].kind == VAR)
                {
                    exptype = EXP;
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = load @";
                    res += lval;
                    res += "\n";
                    val_id ++;
                }
            }
            else
            {
                exptype = NUM;
                // res += "  %";
                // res += std::to_string(val_id);
                // stack[++ top] = val_id;
                // res += " = add 0, ";
                // res += std::to_string(number);
                // res += "\n";
                // val_id ++;
                res += std::to_string(number);
            }
            return res;
        }
        int Val() const override {
            // printf("here is ok!\n");
            if (exp) return exp->Val();
            else if (lval != "") return symtable[lval].value;
            else return number;
        }
};

class UnaryExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> primaryexp;
        std::string unaryop;
        std::unique_ptr<BaseAST> unaryexp;

        std::string Dump() const override {
            std::string res;
            if (!unaryexp)
            {
                res += primaryexp->Dump();
                exptype = primaryexp->exptype;
            }
            else
            {
                exptype = EXP;
                std::string s = unaryexp->Dump();
                if (unaryexp->exptype == EXP)
                {
                    if (unaryop == "+")
                        res += s;
                    else if (unaryop == "-")
                    {
                        res += s;
                        res += "  %";
                        res += std::to_string(val_id);
                        res += " = sub 0, %";
                        res += std::to_string(val_id - 1);
                        res += "\n";
                        val_id ++;
                    }
                    else if (unaryop == "!")
                    {
                        res += s;
                        res += "  %";
                        res += std::to_string(val_id);
                        res += " = eq %";
                        res += std::to_string(val_id - 1);
                        res += ", 0\n";
                        val_id ++;
                    }
                }
                else 
                {
                    if (unaryop == "!")
                    {
                        res += "  %";
                        res += std::to_string(val_id);
                        res += " = eq ";
                        res += s;
                        res += ", 0\n";
                        val_id ++;
                    }
                    else if (unaryop == "-")
                    {
                        res += "  %";
                        res += std::to_string(val_id);
                        res += " = sub 0, ";
                        res += s;
                        res += "\n";
                        val_id ++;
                    }
                    else if (unaryop == "+")
                    {
                        exptype = NUM;
                        res += s;
                    }
                }
            }
            return res;
        }
        int Val() const override {
            if (!unaryexp) return primaryexp->Val();
            else if (unaryop == "+") return unaryexp->Val();
            else if (unaryop == "-") return -unaryexp->Val();
            else if (unaryop == "!") return !unaryexp->Val();
            else {
                printf("There is a bug in UnaryExpAST!\n");
                return -1;
            }
        }
};

class MulExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> unaryexp;
        std::string mulop;
        std::unique_ptr<BaseAST> mulexp;

        std::string Dump() const override {
            std::string res;
            if (!mulexp)
            {
                res += unaryexp->Dump();
                exptype = unaryexp->exptype;
            }
            else
            {
                exptype = EXP; 
                std::string s1, s2;
                int id1, id2;
                s1 += mulexp->Dump();
                if (mulexp->exptype == EXP) 
                {
                    res += s1;
                    id1 = val_id - 1;
                }
                s2 += unaryexp->Dump();
                if (unaryexp->exptype == EXP)
                {
                    res += s2;
                    id2 = val_id - 1;
                }
                res += "  %";
                res += std::to_string(val_id);

                if (mulop == "*")
                    res += " = mul ";
                else if (mulop == "/")
                    res += " = div ";
                else if (mulop == "%")
                    res += " = mod ";
                
                if (mulexp->exptype == EXP)
                {
                    res += "%";
                    res += std::to_string(id1);
                }
                else res += s1;

                res += ", ";

                if (unaryexp->exptype == EXP)
                {
                    res += "%";
                    res += std::to_string(id2);
                }
                else res += s2;
                res += "\n";
                val_id ++;
            }
            return res;
        }
        int Val() const override {
            if (!mulexp) return unaryexp->Val();
            else if (mulop == "*") return mulexp->Val() * unaryexp->Val();
            else if (mulop == "/") return mulexp->Val() / unaryexp->Val();
            else if (mulop == "%") return mulexp->Val() % unaryexp->Val();
            else {
                printf("There is a bug in MulExpAST!\n");
                return -1;
            }
        }
};

class AddExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> mulexp;
        std::string addop;
        std::unique_ptr<BaseAST> addexp;

        std::string Dump() const override {
            std::string res;
            if (!addexp)
            {
                res += mulexp->Dump();
                exptype = mulexp->exptype;
            }
            else
            {
                exptype = EXP;

                std::string s1, s2;
                int id1, id2;
                s1 += addexp->Dump();
                if (addexp->exptype == EXP) 
                {
                    res += s1;
                    id1 = val_id - 1;
                }
                s2 += mulexp->Dump();
                if (mulexp->exptype == EXP)
                {
                    res += s2;
                    id2 = val_id - 1;
                }
                res += "  %";
                res += std::to_string(val_id);

                if (addop == "+")
                    res += " = add ";
                else if (addop == "-")
                    res += " = sub ";
                
                if (addexp->exptype == EXP)
                {
                    res += "%";
                    res += std::to_string(id1);
                }
                else res += s1;

                res += ", ";

                if (mulexp->exptype == EXP)
                {
                    res += "%";
                    res += std::to_string(id2);
                }
                else res += s2;
                res += "\n";
                val_id ++;
            }
            return res;
        }
        int Val() const override {
            if (!addexp) return mulexp->Val();
            else if (addop == "+") return addexp->Val() + mulexp->Val();
            else if (addop == "-") return addexp->Val() - mulexp->Val();
            else 
            {
                printf("There is a bug in AddExpAST!\n");
                return -1;
            }
        }
};

class RelExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> addexp;
        std::string relop;
        std::unique_ptr<BaseAST> relexp;

        std::string Dump() const override {
            std::string res;
            if (!relexp)
            {
                res += addexp->Dump();
                exptype = addexp->exptype;
            }
            else
            {
                exptype = EXP;

                std::string s1, s2;
                int id1, id2;
                s1 += relexp->Dump();
                if (relexp->exptype == EXP) 
                {
                    res += s1;
                    id1 = val_id - 1;
                }
                s2 += addexp->Dump();
                if (addexp->exptype == EXP)
                {
                    res += s2;
                    id2 = val_id - 1;
                }
                res += "  %";
                res += std::to_string(val_id);

                if (relop == "<")
                    res += " = lt ";
                else if (relop == ">")
                    res += " = gt ";
                else if (relop == "<=")
                    res += " = le ";
                else if (relop == ">=")
                    res += " = ge ";
                
                if (relexp->exptype == EXP)
                {
                    res += "%";
                    res += std::to_string(id1);
                }
                else res += s1;

                res += ", ";

                if (addexp->exptype == EXP)
                {
                    res += "%";
                    res += std::to_string(id2);
                }
                else res += s2;
                res += "\n";
                val_id ++;
            }
            return res;
        }
        int Val() const override {
            if (!relexp) return addexp->Val();
            else if (relop == "<") return relexp->Val() < addexp->Val();
            else if (relop == ">") return relexp->Val() > addexp->Val();
            else if (relop == "<=") return relexp->Val() <= addexp->Val();
            else if (relop == ">=") return relexp->Val() >= addexp->Val();
            else 
            {
                printf("There is a bug in RelExpAST!\n");
                return -1;
            }
        }
};

class EqExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> relexp;
        std::string eqop;
        std::unique_ptr<BaseAST> eqexp;

        std::string Dump() const override {
            std::string res;
            if (!eqexp)
            {
                res += relexp->Dump();
                exptype = relexp->exptype;
            }
            else
            {
                exptype = EXP;

                std::string s1, s2;
                int id1, id2;
                s1 += eqexp->Dump();
                if (eqexp->exptype == EXP) 
                {
                    res += s1;
                    id1 = val_id - 1;
                }
                s2 += relexp->Dump();
                if (relexp->exptype == EXP)
                {
                    res += s2;
                    id2 = val_id - 1;
                }
                res += "  %";
                res += std::to_string(val_id);

                if (eqop == "==")
                    res += " = eq ";
                else if (eqop == "!=")
                    res += " = ne ";
                
                if (eqexp->exptype == EXP)
                {
                    res += "%";
                    res += std::to_string(id1);
                }
                else res += s1;

                res += ", ";

                if (relexp->exptype == EXP)
                {
                    res += "%";
                    res += std::to_string(id2);
                }
                else res += s2;
                res += "\n";
                val_id ++;
            }
            return res;
        }
        int Val() const override {
            if (!eqexp) return relexp->Val();
            else if (eqop == "==") return eqexp->Val() == relexp->Val();
            else if (eqop == "!=") return eqexp->Val() != relexp->Val();
            else 
            {
                printf("There is a bug in EqExpAST!\n");
                return -1;
            }
        }
};

class LAndExpAST : public BaseAST{
    public:
        std::unique_ptr<BaseAST> eqexp;
        std::string landop;
        std::unique_ptr<BaseAST> landexp;

        std::string Dump() const override {
            std::string res;
            if (!landexp)
            {
                res += eqexp->Dump();
                exptype = eqexp->exptype;
            }
            else
            {
                exptype = EXP;

                std::string s1, s2;
                int id1, id2;
                s1 += landexp->Dump();
                if (landexp->exptype == EXP)
                {
                    res += s1;
                    id1 = val_id - 1;
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = ne %";
                    res += std::to_string(id1);
                    res += ", 0\n";
                    val_id ++;
                }
                else 
                {
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = ne ";
                    res += s1;
                    res += ", 0\n";
                    val_id ++;
                }
                id1 = val_id - 1;
                s2 += eqexp->Dump();
                if (eqexp->exptype == EXP)
                {
                    res += s2;
                    id2 = val_id - 1;
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = ne %";
                    res += std::to_string(id2);
                    res += ", 0\n";
                    val_id ++;
                }
                else 
                {
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = ne ";
                    res += s2;
                    res += ", 0\n";
                    val_id ++;
                }
                id2 = val_id - 1;
                res += "  %";
                res += std::to_string(val_id);

                if (landop == "&&")
                    res += " = and ";
                else res += "\nThere is a bug.\n";
                
                res += "%";
                res += std::to_string(id1);

                res += ", ";

                res += "%";
                res += std::to_string(id2);
                
                res += "\n";
                val_id ++;
            }
            return res;
        }
        int Val() const override {
            if (!landexp) return eqexp->Val();
            else return landexp->Val() && eqexp->Val();
        }
};

class LOrExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> landexp;
        std::string lorop;
        std::unique_ptr<BaseAST> lorexp;

        std::string Dump() const override {
            std::string res;
            if (!lorexp)
            {
                res += landexp->Dump();
                exptype = landexp->exptype;
            }
            else
            {
                exptype = EXP;

                std::string s1, s2;
                int id1, id2;
                s1 += lorexp->Dump();
                if (lorexp->exptype == EXP)
                {
                    res += s1;
                    id1 = val_id - 1;
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = ne %";
                    res += std::to_string(id1);
                    res += ", 0\n";
                    val_id ++;
                }
                else 
                {
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = ne ";
                    res += s1;
                    res += ", 0\n";
                    val_id ++;
                }
                id1 = val_id - 1;
                s2 += landexp->Dump();
                if (landexp->exptype == EXP)
                {
                    res += s2;
                    id2 = val_id - 1;
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = ne %";
                    res += std::to_string(id2);
                    res += ", 0\n";
                    val_id ++;
                }
                else 
                {
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = ne ";
                    res += s2;
                    res += ", 0\n";
                    val_id ++;
                }
                id2 = val_id - 1;
                res += "  %";
                res += std::to_string(val_id);

                if (lorop == "||")
                    res += " = or ";
                else res += "\nThere is a bug.\n";
                
                res += "%";
                res += std::to_string(id1);

                res += ", ";

                res += "%";
                res += std::to_string(id2);

                res += "\n";
                val_id ++;
            }
            return res;
        }
        int Val() const override {
            if (!lorexp) return landexp->Val();
            else return lorexp->Val() || landexp->Val();
        }
};

#endif