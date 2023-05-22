#ifndef AST_H
#define AST_H
#include <iostream>
#include <cstring>

#define EXP_STACK_SIZE 100

extern int val_id;
extern int max_id;
extern int stack[EXP_STACK_SIZE], top = -1;

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
            std::string res = exp->Dump();
            res += "  ret %";
            res += std::to_string(val_id - 1);
            return res;
        }
};

class ExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> addexp;

        std::string Dump() const override {
            std::string res = addexp->Dump();
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
            }
            else
            {
                res += "  %";
                res += std::to_string(val_id);
                stack[++ top] = val_id;
                res += " = add 0, ";
                res += std::to_string(number);
                res += "\n";
                val_id ++;
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
            if (primaryexp)
            {
                res += primaryexp->Dump();
            }
            else
            {
                if (unaryop == "+")
                    res += unaryexp->Dump();
                else if (unaryop == "-")
                {
                    res += unaryexp->Dump();
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = sub 0, %";
                    res += std::to_string(val_id - 1);
                    res += "\n";
                    val_id ++;
                }
                else if (unaryop == "!")
                {
                    res += unaryexp->Dump();
                    res += "  %";
                    res += std::to_string(val_id);
                    res += " = eq %";
                    res += std::to_string(val_id - 1);
                    res += ", 0\n";
                    val_id ++;
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
            }
            else
            {

                if (mulop == "*")
                    res += " = mul %";
                else if (mulop == "/")
                    res += " = div %";
                else if (mulop == "%")
                    res += " = mod %";
                

                res += mulexp->Dump();
                int id1 = val_id - 1;
                res += unaryexp->Dump();
                int id2 = val_id - 1;
                res += "  %";
                res += std::to_string(val_id);

                res += std::to_string(id1);
                res += ", %";
                res += std::to_string(id2);
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
            }
            else
            {
                res += addexp->Dump();
                int id1 = val_id - 1;
                res += mulexp->Dump();
                int id2 = val_id - 1;
                res += "  %";
                res += std::to_string(val_id);

                if (addop == "+")
                    res += " = add %";
                else if (addop == "-")
                    res += " = sub %";

                res += std::to_string(id1);
                res += ", %";
                res += std::to_string(id2);
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
            if (addexp)
            {
                res += addexp->Dump();
            }
            else
            {
                res += relexp->Dump();
                res += relop;
                res += addexp->Dump();
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
            if (relexp)
            {
                res += relexp->Dump();
            }
            else
            {
                res += eqexp->Dump();
                res += eqop;
                res += relexp->Dump();
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
            if (eqexp)
            {
                res += eqexp->Dump();
            }
            else
            {
                res += landexp->Dump();
                res += landop;
                res += eqexp->Dump();
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
            if (landexp)
            {
                res += landexp->Dump();
            }
            else
            {
                res += lorexp->Dump();
                res += lorop;
                res += landexp->Dump();
            }
            return res;
        }
};

#endif