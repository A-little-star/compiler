%code requires {
  #include <memory>
  #include <string>
  #include "AST.hpp"
}

%{

#include <iostream>
#include <memory>
#include <string>
#include "AST.hpp"

// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

using namespace std;

%}

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::unique_ptr<BaseAST> &ast }

// yylval 的定义, 我们把它定义成了一个联合体 (union)
// 因为 token 的值有的是字符串指针, 有的是整数
// 之前我们在 lexer 中用到的 str_val 和 int_val 就是在这里被定义的
// 至于为什么要用字符串指针而不直接用 string 或者 unique_ptr<string>?
// 请自行 STFW 在 union 里写一个带析构函数的类会出现什么情况
%union {
  std::string *str_val;
  int int_val;
  BaseAST *ast_val;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token INT RETURN CONST IF ELSE WHILE BREAK CONTINUE FOR
%token <str_val> IDENT LTOP GTOP EQOP NEQOP ANDOP OROP
%token <int_val> INT_CONST

// 非终结符的类型定义
%type <ast_val> FuncDef FuncType Block Stmt OpenStmt ClosedStmt NonIfStmt LessStmt Exp PrimaryExp UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp BlockItems BlockItem Decl ConstDecl ConstDefs ConstDef ConstInitVal ConstExp
%type <ast_val> VarDecl VarDefs VarDef InitVal
%type <str_val> UnaryOp RelOp EqOp LAndOp LOrOp BType LVal
%type <int_val> Number

%%

// 开始符, CompUnit ::= FuncDef, 大括号后声明了解析完成后 parser 要做的事情
// 之前我们定义了 FuncDef 会返回一个 str_val, 也就是字符串指针
// 而 parser 一旦解析完 CompUnit, 就说明所有的 token 都被解析了, 即解析结束了
// 此时我们应该把 FuncDef 返回的结果收集起来, 作为 AST 传给调用 parser 的函数
// $1 指代规则里第一个符号的返回值, 也就是 FuncDef 的返回值
CompUnit
  : FuncDef {
    auto comp_unit = make_unique<CompUnitAST>();
    comp_unit->func_def = unique_ptr<BaseAST>($1);
    ast = move(comp_unit);
  }
  ;

// FuncDef ::= FuncType IDENT '(' ')' Block;
// 我们这里可以直接写 '(' 和 ')', 因为之前在 lexer 里已经处理了单个字符的情况
// 解析完成后, 把这些符号的结果收集起来, 然后拼成一个新的字符串, 作为结果返回
// $$ 表示非终结符的返回值, 我们可以通过给这个符号赋值的方法来返回结果
// 你可能会问, FuncType, IDENT 之类的结果已经是字符串指针了
// 为什么还要用 unique_ptr 接住它们, 然后再解引用, 把它们拼成另一个字符串指针呢
// 因为所有的字符串指针都是我们 new 出来的, new 出来的内存一定要 delete
// 否则会发生内存泄漏, 而 unique_ptr 这种智能指针可以自动帮我们 delete
// 虽然此处你看不出用 unique_ptr 和手动 delete 的区别, 但当我们定义了 AST 之后
// 这种写法会省下很多内存管理的负担
FuncDef
  : FuncType IDENT '(' ')' Block {
    auto ast = new FuncDefAST();
    ast->func_type = unique_ptr<BaseAST>($1);
    ast->ident = *unique_ptr<string>($2);
    ast->block = unique_ptr<BaseAST>($5);
    $$ = ast;
  }
  ;

// 同上, 不再解释
FuncType
  : INT {
    auto ast = new FuncTypeAST();
    ast->type = "int";
    $$ = ast;
  }
  ;

Block
  : '{' BlockItems '}' {
    auto ast = new BlockAST();
    ast->blockitems = unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  | '{' '}' {
    auto ast = new BlockAST();
    ast->blockitems = NULL;
    $$ = ast;
  }
  ;

BlockItems
  : BlockItems BlockItem {
    BlockItemsAST *ast = (BlockItemsAST*)($1);
    ast->blockitems.emplace_back(unique_ptr<BaseAST>($2));
    $$ = ast;
  }
  | BlockItem {
    auto ast = new BlockItemsAST();
    ast->blockitems.emplace_back(unique_ptr<BaseAST>($1));
    $$ = ast;
  }
  ;

BlockItem
  : Decl {
    auto ast = new BlockItemAST();
    ast->type = BlockItemAST::DECL;
    ast->decl = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | Stmt {
    auto ast = new BlockItemAST();
    ast->type = BlockItemAST::STMT;
    ast->stmt = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

Decl
  : ConstDecl {
    auto ast = new DeclAST();
    ast->type = DeclAST::CON_DECL;
    ast->constdecl = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | VarDecl {
    auto ast = new DeclAST();
    ast->type = DeclAST::VAR_DECL;
    ast->vardecl = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

ConstDecl
  : CONST BType ConstDefs ';' {
    auto ast = new ConstDeclAST();
    ast->btype = *unique_ptr<string>($2);
    ast->constdefs = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

BType
  : INT {
    $$ = new string("int");
  }
  ;

ConstDefs
  : ConstDefs ',' ConstDef {
    ConstDefsAST *ast = (ConstDefsAST*)($1);
    ast->constdefs.emplace_back(unique_ptr<BaseAST>($3));
    $$ = ast;
  }
  | ConstDef {
    auto ast = new ConstDefsAST();
    ast->constdefs.emplace_back(unique_ptr<BaseAST>($1));
    $$ = ast;
  }
  ;

ConstDef
  : IDENT '=' ConstInitVal {
    auto ast = new ConstDefAST();
    ast->ident = *unique_ptr<string>($1);
    ast->constinitval = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

ConstInitVal
  : ConstExp {
    auto ast = new ConstInitValAST();
    ast->constexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

VarDecl
  : BType VarDefs ';' {
    auto ast = new VarDeclAST();
    ast->btype = *unique_ptr<string>($1);
    ast->vardefs = unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  ;

VarDefs
  : VarDefs ',' VarDef {
    VarDefsAST *ast = (VarDefsAST*)($1);
    ast->vardefs.emplace_back(unique_ptr<BaseAST>($3));
    $$ = ast;
  }
  | VarDef {
    auto ast = new VarDefsAST();
    ast->vardefs.emplace_back(unique_ptr<BaseAST>($1));
    $$ = ast;
  }
  ;

VarDef
  : IDENT {
    auto ast = new VarDefAST();
    ast->type = VarDefAST::NO_VALUE;
    ast->ident = *unique_ptr<string>($1);
    $$ = ast;
  }
  | IDENT '=' InitVal {
    auto ast = new VarDefAST();
    ast->type = VarDefAST::HAS_VALUE;
    ast->ident = *unique_ptr<string>($1);
    ast->initval = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

InitVal
  : Exp {
    auto ast = new InitValAST();
    ast->exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

Stmt
  : OpenStmt {
    auto ast = new StmtAST();
    ast->type = StmtAST::OPENSTMT;
    ast->openstmt = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | ClosedStmt {
    auto ast = new StmtAST();
    ast->type = StmtAST::CLOSEDSTMT;
    ast->closedstmt = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

OpenStmt
  : IF '(' Exp ')' Stmt {
    auto ast = new OpenStmtAST();
    ast->type = OpenStmtAST::IF;
    ast->exp = unique_ptr<BaseAST>($3);
    ast->stmt = unique_ptr<BaseAST>($5);
    $$ = ast;
  }
  | IF '(' Exp ')' ClosedStmt ELSE OpenStmt {
    auto ast = new OpenStmtAST();
    ast->type = OpenStmtAST::IF_ELSE;
    ast->exp = unique_ptr<BaseAST>($3);
    ast->closedstmt = unique_ptr<BaseAST>($5);
    ast->openstmt = unique_ptr<BaseAST>($7);
    $$ = ast;
  }
  ;

ClosedStmt
  : NonIfStmt {
    auto ast = new ClosedStmtAST();
    ast->type = ClosedStmtAST::NONIF;
    ast->nonifstmt = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | IF '(' Exp ')' ClosedStmt ELSE ClosedStmt {
    auto ast = new ClosedStmtAST();
    ast->type = ClosedStmtAST::IF_ELSE;
    ast->exp = unique_ptr<BaseAST>($3);
    ast->closedstmt_if = unique_ptr<BaseAST>($5);
    ast->closedstmt_else = unique_ptr<BaseAST>($7);
    $$ = ast;
  }
  ;

NonIfStmt
  : LessStmt ';' {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::STMT;
    ast->lessstmt = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | Block {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::BLOCK;
    ast->block = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | WHILE '(' Exp ')' Stmt {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::WHILE;
    ast->exp = unique_ptr<BaseAST>($3);
    ast->stmt = unique_ptr<BaseAST>($5);
    $$ = ast;
  }
  | FOR '(' BlockItem  Exp ';' LessStmt ')' Stmt {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::FOR;
    ast->blockitem = unique_ptr<BaseAST>($3);
    ast->exp = unique_ptr<BaseAST>($4);
    ast->lessstmt = unique_ptr<BaseAST>($6);
    ast->stmt = unique_ptr<BaseAST>($8);
    $$ = ast;
  }
  ;

LessStmt
  : RETURN Exp {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::RETURN;
    ast->ret = "return";
    ast->exp = unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  | RETURN {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::RETURN;
    ast->ret = "return";
    ast->exp = NULL;
    $$ = ast;
  }
  | LVal '=' Exp {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::ASSIGN;
    ast->lval = *unique_ptr<string>($1);
    ast->exp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  | Exp {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::VOID;
    ast->exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::VOID;
    ast->exp = NULL;
    $$ = ast;
  }
  | BREAK {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::BREAK;
    $$ = ast;
  }
  | CONTINUE {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::CONTINUE;
    $$ = ast;
  }
  ;

ConstExp
  : Exp {
    auto ast = new ConstExpAST();
    ast->exp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

Exp
  : LOrExp {
    auto ast = new ExpAST();
    ast->lorexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  ;

PrimaryExp
  : '(' Exp ')' {
    auto ast = new PrimaryExpAST();
    ast->type = PrimaryExpAST::EXP;
    ast->exp = unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  | LVal {
    auto ast = new PrimaryExpAST();
    ast->type = PrimaryExpAST::LVAL;
    ast->lval = *unique_ptr<string>($1);
    $$ = ast;
  }
  | Number {
    auto ast = new PrimaryExpAST();
    ast->type = PrimaryExpAST::NUM;
    ast->number = $1;
    $$ = ast;
  }
  ;

LVal
  : IDENT {
    $$ = $1;
  }
  ;

UnaryExp
  : PrimaryExp {
    auto ast = new UnaryExpAST();
    ast->type = UnaryExpAST::NAN;
    ast->primaryexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | UnaryOp UnaryExp {
    auto ast = new UnaryExpAST();
    ast->type = UnaryExpAST::EXP;
    ast->unaryop = *unique_ptr<string>($1);
    ast->unaryexp = unique_ptr<BaseAST>($2);
    $$ = ast;
  }
  ;

UnaryOp
  : '+' { $$ = new string("+"); }
  | '-' { $$ = new string("-"); }
  | '!' { $$ = new string("!"); }
  ;

MulExp
  : UnaryExp {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::NAN;
    ast->unaryexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | MulExp '*' UnaryExp {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::EXP;
    ast->mulexp = unique_ptr<BaseAST>($1);
    ast->unaryexp = unique_ptr<BaseAST>($3);
    ast->mulop = *unique_ptr<string>(new string("*"));
    $$ = ast;
  }
  | MulExp '/' UnaryExp {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::EXP;
    ast->mulexp = unique_ptr<BaseAST>($1);
    ast->unaryexp = unique_ptr<BaseAST>($3);
    ast->mulop = *unique_ptr<string>(new string("/"));
    $$ = ast;
  }
  | MulExp '%' UnaryExp {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::EXP;
    ast->mulexp = unique_ptr<BaseAST>($1);
    ast->unaryexp = unique_ptr<BaseAST>($3);
    ast->mulop = *unique_ptr<string>(new string("%"));
    $$ = ast;
  }
  ;

AddExp
  : MulExp {
    auto ast = new AddExpAST();
    ast->type = AddExpAST::NAN;
    ast->mulexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | AddExp '+' MulExp {
    auto ast = new AddExpAST();
    ast->type = AddExpAST::EXP;
    ast->addexp = unique_ptr<BaseAST>($1);
    ast->mulexp = unique_ptr<BaseAST>($3);
    ast->addop = *unique_ptr<string>(new string("+"));
    $$ = ast;
  }
  | AddExp '-' MulExp {
    auto ast = new AddExpAST();
    ast->type = AddExpAST::EXP;
    ast->addexp = unique_ptr<BaseAST>($1);
    ast->mulexp = unique_ptr<BaseAST>($3);
    ast->addop = *unique_ptr<string>(new string("-"));
    $$ = ast;
  }
  ;

Number
  : INT_CONST {
    $$ = $1;
    // $$ = new string(to_string($1));
  }
  ;

RelExp
  : AddExp {
    auto ast = new RelExpAST();
    ast->type = RelExpAST::NAN;
    ast->addexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | RelExp RelOp AddExp {
    auto ast = new RelExpAST();
    ast->type = RelExpAST::EXP;
    ast->relexp = unique_ptr<BaseAST>($1);
    ast->relop = *unique_ptr<string>($2);
    ast->addexp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

RelOp
  : '<' { $$ = new string("<"); }
  | '>' { $$ = new string(">"); }
  | LTOP { $$ = new string("<="); }
  | GTOP { $$ = new string(">="); }
  ;

EqExp
  : RelExp {
    auto ast = new EqExpAST();
    ast->type = EqExpAST::NAN;
    ast->relexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | EqExp EqOp RelExp {
    auto ast = new EqExpAST();
    ast->type = EqExpAST::EXP;
    ast->eqexp = unique_ptr<BaseAST>($1);
    ast->eqop = *unique_ptr<string>($2);
    ast->relexp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

EqOp
  : EQOP { $$ = new string("=="); }
  | NEQOP { $$ = new string("!="); }
  ;

LAndExp
  : EqExp {
    auto ast = new LAndExpAST();
    ast->type = LAndExpAST::NAN;
    ast->eqexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | LAndExp LAndOp EqExp {
    auto ast = new LAndExpAST();
    ast->type = LAndExpAST::EXP;
    ast->landexp = unique_ptr<BaseAST>($1);
    ast->landop = *unique_ptr<string>($2);
    ast->eqexp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

LAndOp
  : ANDOP { $$ = new string("&&"); }
  ;

LOrExp
  : LAndExp {
    auto ast = new LOrExpAST();
    ast->type = LOrExpAST::NAN;
    ast->landexp = unique_ptr<BaseAST>($1);
    $$ = ast;
  }
  | LOrExp LOrOp LAndExp {
    auto ast = new LOrExpAST();
    ast->type = LOrExpAST::EXP;
    ast->lorexp = unique_ptr<BaseAST>($1);
    ast->lorop = *unique_ptr<string>($2);
    ast->landexp = unique_ptr<BaseAST>($3);
    $$ = ast;
  }
  ;

LOrOp
  : OROP { $$ = new string("||"); }
  ;

%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}