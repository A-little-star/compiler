#ifndef AST_H
#define AST_H
#include <iostream>
#include <cstring>

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
            else return "non-type";
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
        std::unique_ptr<BaseAST> exp;

        std::string Dump() const override {
            std::string res;
            std::string s = exp->Dump();

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
            return res;
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
};

class PrimaryExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> exp;
        int number;

        std::string Dump() const override {
            std::string res;
            if (exp)
            {
                res += exp->Dump();
                exptype = EXP;
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
};

#endif