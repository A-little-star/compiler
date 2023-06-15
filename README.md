# SysY程序到risc-v汇编的编译器

本仓库记录lxc编写的第一个编译器，于2023年五月开始编写；

该编译器使用的中间表示为Koopa IR

SysY语言是一种面向系统的高级编程语言，是一个简化的C语言子集，具有较为简单的语法和语义规则。

Koopa IR是参考北京大学编译实践在线文档（https://pku-minic.github.io/online-doc/#/）的一种中间表示，它在形式上类似于LLVM IR，但简化了很多内容，只专注于需要的部分。


## 目前所支持的文法规则

```
CompUnit    ::= FuncDef;

FuncDef     ::= FuncType IDENT "(" ")" Block;
FuncType    ::= "int";

Block       ::= "{" Stmt "}";
Stmt        ::= "return" Exp ";";

Exp         ::= LOrExp;
PrimaryExp  ::= "(" Exp ")" | Number;
Number      ::= INT_CONST;
UnaryExp    ::= PrimaryExp | UnaryOp UnaryExp;
UnaryOp     ::= "+" | "-" | "!";
MulExp      ::= UnaryExp | MulExp ("*" | "/" | "%") UnaryExp;
AddExp      ::= MulExp | AddExp ("+" | "-") MulExp;
RelExp      ::= AddExp | RelExp ("<" | ">" | "<=" | ">=") AddExp;
EqExp       ::= RelExp | EqExp ("==" | "!=") RelExp;
LAndExp     ::= EqExp | LAndExp "&&" EqExp;
LOrExp      ::= LAndExp | LOrExp "||" LAndExp;
```

## IR
生成抽象语法树AST之后，编译器会遍历AST，生成数据结构形式的Koopa IR，生成的过程写在midend/ast2ir.cpp中。

