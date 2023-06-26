# SysY程序到risc-v汇编的编译器


## Bug list
1.（2023.6.26, 未解决）目前IR中的变量类型（alloc类型）的type（ty）没有正确赋值，一个变量的alloc类型应该是一个pointer类型。

同时，应该修改load，store指令中，对指针类型的处理。

2.（2023.6.27，未解决）目前全局作用域内的数组还不能够正常引用，推测数据结构形式的IR可以正常生成，但是无法正常输出。

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
NonIfStmt     ::= LessStmt ";"
                | Block
                | "while" "(" Exp ")" Stmt
                | "for" "(" BlockItem ";" Exp ";" LessStmt ")" Stmt; 
LessStmt     ::= LVal "=" Exp
                | [Exp]
                | "return" [Exp]
                | "break"
                | "continue";

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



