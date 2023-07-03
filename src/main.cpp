#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include "./frontend/AST.hpp"
#include "./midend/xc.hpp"
#include "./midend/ast2ir.hpp"
#include "./backend/riscv.hpp"

using namespace std;

// 声明 lexer 的输入, 以及 parser 函数
// 为什么不引用 sysy.tab.hpp 呢? 因为首先里面没有 yyin 的定义
// 其次, 因为这个文件不是我们自己写的, 而是被 Bison 生成出来的
// 你的代码编辑器/IDE 很可能找不到这个文件, 然后会给你报错 (虽然编译不会出错)
// 看起来会很烦人, 于是干脆采用这种看起来 dirty 但实际很有效的手段
extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);
int val_id = 0;
int max_id = 0;

// prog_ptr prog = NULL;
RiscvProgram *riscvprogram;

int main(int argc, const char *argv[]) {
  // 解析命令行参数. 测试脚本/评测平台要求你的编译器能接收如下参数:
  // compiler 模式 输入文件 -o 输出文件
  assert(argc == 5);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  assert(yyin);

  // 调用 parser 函数, parser 函数会进一步调用 lexer 解析输入文件的
  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  assert(!ret);
  printf("AST is built successfully\n");

  if (strcmp(mode, "-ast") == 0) {
    ast->Dump();
  }
  else if (strcmp(mode, "-koopa") == 0) {
    // 构造一个生成IR的Visitor类，调用accept()方法生成数据结构形式的koopa IR
    auto v = new GenIR();
    prog_ptr prog = (prog_ptr)ast->accept(v);

    printf("Koopa IR is built successfully!\n");

    // 遍历数据结构形式的koopa IR，转化成文本形式输出到output文件中
    ofstream file_o(output);
    if (file_o.is_open()) {
      irDS2Text(prog, file_o);
      file_o.close();
    }
    else 
      cout << "main.cpp: Unable to create the output file." << endl;

    // 将output文件中的内容打印出来显示
    ifstream file_i(output);
    if (file_i.is_open()) {
      string line;
      while (getline(file_i, line)) {
        cout << line << endl;
      }
      file_i.close();
    }
    else
      cout << "main.cpp: Unable to create the input file." << endl;

    // 释放IR所占用的内存
    delete v;
    v = NULL;
    FreeMem(prog);
  }
  else if (strcmp(mode, "-riscv") == 0) {
    // 构造一个生成IR的Visitor类，调用accept()方法生成数据结构形式的koopa IR
    auto v = new GenIR();
    prog_ptr prog = (prog_ptr)ast->accept(v);

    // 遍历数据结构形式的IR，生成RISC-V汇编
    ofstream file_o(output);
    if (file_o.is_open()) {
      ir2riscv(prog, std::cout);
      riscvprogram->Dump(file_o);
      file_o.close();
    }
    else 
      cout << "main.cpp: Unable to create the output file." << endl;

    // 将output文件中的内容打印出来显示
    // riscvprogram->Dump(std::cout);
    // ifstream file_i(output);
    // if (file_i.is_open()) {
    //   string line;
    //   while (getline(file_i, line)) {
    //     cout << line << endl;
    //   }
    //   file_i.close();
    // }
    // else
    //   cout << "main.cpp: Unable to create the input file." << endl;
    
    delete v;
    v = NULL;
    FreeMem(prog);
  }
  return 0;
}
