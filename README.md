# SysY程序到risc-v汇编的编译器


## Bug list
1.（2023.6.26, 已解决）目前IR中的变量类型（alloc类型）的type（ty）没有正确赋值，一个变量的alloc类型应该是一个pointer类型。

同时，应该修改load，store指令中，对指针类型的处理。

2.（2023.6.27，已解决）目前全局作用域内的数组还不能够正常引用，推测数据结构形式的IR可以正常生成，但是无法正常输出。

3.（2023.7.1，已解决）Koopa IR的类型应该是这样一个逻辑，一个某类型的alloc变量，应该是该类型的指针类型，比如%1 = alloc i32，则%1的类型是\*i32；%2 = alloc [i32, 5]，则%2的类型是\*[i32, 5]；getptr指令不会改变数据的类型，只是指针的偏移运算而已；getelemptr会将数组类型的指针转化成数组的元素类型的指针，如%1的类型是*[i32, 5]，则经过getelemptr指令（%2 = getelemptr %1, 0），%2将会变成*i32类型。这里的类型处理没有处理好

4.（2023.7.2，<font color=Red>未解决</font>）目前数组的初始化只支持无初始化列表、完全初始化列表两种情形，其他情况过于复杂，暂不支持。

5.（2023.7.2，<font color=Red>未解决</font>）当一个函数在所有的控制分支中都有return指令时，会出现一个空的基本块，该基本块目前没有在数据结构层面删掉，而是在打印时选择不输出。

6.（2023.7.2，已解决）由于之前的参数分配策略是懒惰策略，也就是只有用到该形参时，才会为该形参分配空间，这样会出现一个问题：有可能分配形参会出现在使用该形参的“后面”，这是由于基本块的分布顺序与指令的生成顺序是不同的。采用的解决办法是：在函数的开头直接为所有的形参分配空间即可。

## SysY Compiler

本仓库记录lxc编写的第一个编译器，于2023年五月开始编写；

该编译器使用的中间表示为Koopa IR

SysY语言是一种面向系统的高级编程语言，是一个简化的C语言子集，具有较为简单的语法和语义规则。

Koopa IR是参考北京大学编译实践在线文档（ https://pku-minic.github.io/online-doc/#/ ）的一种中间表示，它在形式上类似于LLVM IR，但简化了很多内容，只专注于需要的部分。


## 目前所支持的文法规则

```enbf
CompUnit      ::= [CompUnit] (Decl | FuncDef);

Decl          ::= ConstDecl | VarDecl;
ConstDecl     ::= "const" BType ConstDef {"," ConstDef} ";";
BType         ::= "int";
ConstDef      ::= IDENT {"[" ConstExp "]"} "=" ConstInitVal;
ConstInitVal  ::= ConstExp | "{" [ConstInitVal {"," ConstInitVal}] "}";
VarDecl       ::= BType VarDef {"," VarDef} ";";
VarDef        ::= IDENT {"[" ConstExp "]"}
                | IDENT {"[" ConstExp "]"} "=" InitVal;
InitVal       ::= Exp | "{" [InitVal {"," InitVal}] "}";

FuncDef       ::= FuncType IDENT "(" [FuncFParams] ")" Block;
FuncType      ::= "void" | "int";
FuncFParams   ::= FuncFParam {"," FuncFParam};
FuncFParam    ::= BType IDENT ["[" "]" {"[" ConstExp "]"}];

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
LVal          ::= IDENT {"[" Exp "]"};
PrimaryExp    ::= "(" Exp ")" | LVal | Number;
Number        ::= INT_CONST;
UnaryExp      ::= PrimaryExp | IDENT "(" [FuncRParams] ")" | UnaryOp UnaryExp;
UnaryOp       ::= "+" | "-" | "!";
FuncRParams   ::= Exp {"," Exp};
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

## Risc-V汇编
汇编的生成方式目前是遍历Koopa IR直接生成文本形式的汇编代码，且目前没有经过任何寄存器的分配策略，这里还需要进行优化。

