# SysY程序到risc-v汇编的编译器

本仓库记录lxc编写的第一个编译器，于2023年五月开始编写；

该编译器使用的中间表示为Koopa IR

SysY语言是一种面向系统的高级编程语言，是一个简化的C语言子集，具有较为简单的语法和语义规则。

Koopa IR是参考北京大学编译实践在线文档（ https://pku-minic.github.io/online-doc/#/ ）的一种中间表示，它在形式上类似于LLVM IR，但简化了很多内容，只专注于需要的部分。


## 目前所支持的文法规则

```enbf
CompUnit      ::= FuncDef;

Decl          ::= ConstDecl | VarDecl;
ConstDecl     ::= "const" BType ConstDef {"," ConstDef} ";";
BType         ::= "int";
ConstDef      ::= IDENT "=" ConstInitVal;
ConstInitVal  ::= ConstExp;
VarDecl       ::= BType VarDef {"," VarDef} ";";
VarDef        ::= IDENT | IDENT "=" InitVal;
InitVal       ::= Exp;

FuncDef       ::= FuncType IDENT "(" ")" Block;
FuncType      ::= "int";

Block         ::= "{" {BlockItem} "}";
BlockItem     ::= Decl | Stmt;

Stmt          ::= OpenStmt
                | ClosedStmt
OpenStmt      ::= IF "(" Exp ")" Stmt
                | IF "(" Exp ")" ClosedStmt ELSE OpenStmt
ClosedStmt    ::= NonIfStmt
                | IF "(" Exp ")" ClosedStmt ElSE ClosedStmt
NonIfStmt     ::= LVal "=" Exp ";"
                | [Exp] ";"
                | Block
                | "return" [Exp] ";";

Exp           ::= LOrExp;
LVal          ::= IDENT;
PrimaryExp    ::= "(" Exp ")" | LVal | Number;
Number        ::= INT_CONST;
UnaryExp      ::= PrimaryExp | UnaryOp UnaryExp;
UnaryOp       ::= "+" | "-" | "!";
MulExp        ::= UnaryExp | MulExp ("*" | "/" | "%") UnaryExp;
AddExp        ::= MulExp | AddExp ("+" | "-") MulExp;
RelExp        ::= AddExp | RelExp ("<" | ">" | "<=" | ">=") AddExp;
EqExp         ::= RelExp | EqExp ("==" | "!=") RelExp;
LAndExp       ::= EqExp | LAndExp "&&" EqExp;
LOrExp        ::= LAndExp | LOrExp "||" LAndExp;
ConstExp      ::= Exp;

```

## IR
生成抽象语法树AST之后，编译器会遍历AST，生成数据结构形式的Koopa IR，生成的过程写在midend/ast2ir.cpp中。

