/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 7 "/root/compiler/src/frontend/sysy.y"


#include <iostream>
#include <memory>
#include <string>
#include "AST.hpp"

// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

using namespace std;


#line 85 "/root/compiler/build/frontend/sysy.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_ROOT_COMPILER_BUILD_FRONTEND_SYSY_TAB_HPP_INCLUDED
# define YY_YY_ROOT_COMPILER_BUILD_FRONTEND_SYSY_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "/root/compiler/src/frontend/sysy.y"

  #include <memory>
  #include <string>
  #include "AST.hpp"

#line 134 "/root/compiler/build/frontend/sysy.tab.cpp"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    RETURN = 259,
    CONST = 260,
    IF = 261,
    ELSE = 262,
    WHILE = 263,
    BREAK = 264,
    CONTINUE = 265,
    FOR = 266,
    VOID = 267,
    IDENT = 268,
    LTOP = 269,
    GTOP = 270,
    EQOP = 271,
    NEQOP = 272,
    ANDOP = 273,
    OROP = 274,
    INT_CONST = 275
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 32 "/root/compiler/src/frontend/sysy.y"

  std::string *str_val;
  int int_val;
  BaseAST *ast_val;

#line 172 "/root/compiler/build/frontend/sysy.tab.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (std::unique_ptr<BaseAST> &ast);

#endif /* !YY_YY_ROOT_COMPILER_BUILD_FRONTEND_SYSY_TAB_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   184

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  36
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  89
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  145

#define YYUNDEFTOK  2
#define YYMAXUTOK   275


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,     2,     2,    33,     2,     2,
      21,    22,    31,    28,    23,    29,     2,    32,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    26,
      34,    27,    35,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    24,     2,    25,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    58,    58,    66,    71,    79,    85,   104,   113,   126,
     131,   139,   144,   152,   161,   166,   174,   179,   187,   193,
     202,   208,   217,   226,   232,   237,   245,   254,   262,   271,
     276,   284,   290,   300,   308,   314,   323,   330,   341,   347,
     358,   364,   370,   377,   389,   396,   403,   410,   416,   422,
     427,   435,   443,   451,   457,   463,   472,   478,   484,   491,
     497,   507,   512,   520,   521,   522,   526,   532,   540,   548,
     559,   565,   573,   584,   591,   597,   608,   609,   610,   611,
     615,   621,   632,   633,   637,   643,   654,   658,   664,   675
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "RETURN", "CONST", "IF", "ELSE",
  "WHILE", "BREAK", "CONTINUE", "FOR", "VOID", "IDENT", "LTOP", "GTOP",
  "EQOP", "NEQOP", "ANDOP", "OROP", "INT_CONST", "'('", "')'", "','",
  "'{'", "'}'", "';'", "'='", "'+'", "'-'", "'!'", "'*'", "'/'", "'%'",
  "'<'", "'>'", "$accept", "CompUnit", "CompItems", "CompItem", "FuncDef",
  "FuncType", "FuncFParams", "FuncFParam", "Block", "BlockItems",
  "BlockItem", "Decl", "ConstDecl", "BType", "ConstDefs", "ConstDef",
  "ConstInitVal", "VarDecl", "VarDefs", "VarDef", "InitVal", "Stmt",
  "OpenStmt", "ClosedStmt", "NonIfStmt", "LessStmt", "ConstExp", "Exp",
  "PrimaryExp", "LVal", "UnaryExp", "FuncRParams", "UnaryOp", "MulExp",
  "AddExp", "Number", "RelExp", "RelOp", "EqExp", "EqOp", "LAndExp",
  "LAndOp", "LOrExp", "LOrOp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,    40,    41,    44,   123,   125,    59,    61,    43,    45,
      33,    42,    47,    37,    60,    62
};
# endif

#define YYPACT_NINF (-126)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      51,  -126,    35,  -126,    10,    51,  -126,  -126,    31,  -126,
    -126,    36,  -126,  -126,    42,  -126,  -126,    39,    65,    47,
    -126,    68,    53,  -126,     3,   154,    36,  -126,   154,    42,
    -126,    58,    49,  -126,    83,    76,  -126,   154,  -126,  -126,
    -126,  -126,  -126,  -126,  -126,  -126,   154,   -10,    55,  -126,
      54,    61,    80,    81,  -126,  -126,  -126,  -126,  -126,   102,
    -126,    58,    35,  -126,   148,    87,  -126,   154,   154,   154,
     154,   154,  -126,  -126,  -126,  -126,   154,  -126,  -126,   154,
    -126,   154,  -126,   154,   154,    82,    93,  -126,  -126,    95,
    -126,  -126,   130,  -126,  -126,  -126,  -126,  -126,  -126,    91,
    -126,    75,  -126,  -126,  -126,  -126,    64,  -126,  -126,  -126,
    -126,   -10,   -10,    55,    54,    61,    80,  -126,   154,   154,
      37,  -126,  -126,  -126,   154,  -126,   154,    96,    97,   154,
    -126,  -126,     7,     7,    94,  -126,   114,  -126,   143,     7,
     103,  -126,  -126,     7,  -126
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     9,     0,    10,     0,     2,     4,     6,     0,     5,
      20,     0,    21,    23,     0,     1,     3,     0,    31,     0,
      30,     0,     0,    25,     0,     0,     0,    28,     0,     0,
      22,     0,     0,    12,     0,    56,    73,     0,    63,    64,
      65,    32,    33,    57,    54,    66,     0,    70,    74,    55,
      80,    84,    87,    52,    29,    26,    27,    51,    24,    48,
       7,     0,     0,    13,     0,     0,    58,     0,     0,     0,
       0,     0,    78,    79,    76,    77,     0,    82,    83,     0,
      86,     0,    89,     0,    45,     0,     0,    49,    50,     0,
      15,    41,    48,    17,    18,    19,    34,    35,    38,     0,
      47,    54,     8,    11,    59,    62,     0,    53,    67,    68,
      69,    71,    72,    75,    81,    85,    88,    44,     0,     0,
      48,    14,    16,    40,     0,    60,     0,     0,     0,     0,
      46,    61,    48,    48,     0,    36,    35,    42,    48,    48,
       0,    37,    39,    48,    43
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -126,  -126,  -126,   119,  -126,  -126,  -126,    66,   -29,  -126,
     -87,    24,  -126,     2,  -126,   100,  -126,  -126,  -126,   111,
    -126,  -124,     5,  -125,  -126,     4,  -126,   -25,  -126,   -58,
     -16,  -126,  -126,    20,    69,  -126,    67,  -126,    84,  -126,
      74,  -126,  -126,  -126
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,     8,    32,    33,    91,    92,
      93,    94,    10,    11,    22,    23,    55,    12,    19,    20,
      41,    95,    96,    97,    98,    99,    56,   100,    43,    44,
      45,   106,    46,    47,    48,    49,    50,    76,    51,    79,
      52,    81,    53,    83
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      42,   101,    60,    57,    14,   122,    13,   136,   135,   137,
      15,    84,    65,    85,   142,    86,    87,    88,    89,   144,
      35,    67,    68,    69,     9,    31,    34,    36,    37,     9,
      66,    59,   102,   129,   101,    38,    39,    40,    13,   105,
      13,    84,     2,    85,    17,    86,    87,    88,    89,    18,
      35,   108,   109,   110,     1,    21,     2,    36,    37,   117,
      24,    59,   101,     3,    34,    38,    39,    40,    72,    73,
      26,    61,    62,    27,   101,   101,    29,    77,    78,    30,
     101,   101,    59,    70,    71,   101,   125,   126,    74,    75,
     111,   112,    25,   127,   128,    28,    63,    64,    80,   130,
      82,   131,   124,   118,   134,    13,    84,     2,    85,   107,
      86,    87,    88,    89,   119,    35,   120,   123,   132,   133,
     138,   139,    36,    37,    16,   143,    59,    90,   103,    58,
      38,    39,    40,    13,    84,     2,    85,    54,    86,    87,
      88,    89,   140,    35,   141,   113,   114,    84,     0,     0,
      36,    37,    87,    88,    59,   121,    35,   116,    38,    39,
      40,    35,     0,    36,    37,   115,     0,    35,    36,    37,
     104,    38,    39,    40,    36,    37,    38,    39,    40,     0,
       0,     0,    38,    39,    40
};

static const yytype_int16 yycheck[] =
{
      25,    59,    31,    28,     2,    92,     3,   132,   132,   133,
       0,     4,    37,     6,   139,     8,     9,    10,    11,   143,
      13,    31,    32,    33,     0,    22,    24,    20,    21,     5,
      46,    24,    61,   120,    92,    28,    29,    30,     3,    64,
       3,     4,     5,     6,    13,     8,     9,    10,    11,    13,
      13,    67,    68,    69,     3,    13,     5,    20,    21,    84,
      21,    24,   120,    12,    62,    28,    29,    30,    14,    15,
      23,    22,    23,    26,   132,   133,    23,    16,    17,    26,
     138,   139,    24,    28,    29,   143,    22,    23,    34,    35,
      70,    71,    27,   118,   119,    27,    13,    21,    18,   124,
      19,   126,    27,    21,   129,     3,     4,     5,     6,    22,
       8,     9,    10,    11,    21,    13,    21,    26,    22,    22,
      26,     7,    20,    21,     5,    22,    24,    25,    62,    29,
      28,    29,    30,     3,     4,     5,     6,    26,     8,     9,
      10,    11,   138,    13,   139,    76,    79,     4,    -1,    -1,
      20,    21,     9,    10,    24,    25,    13,    83,    28,    29,
      30,    13,    -1,    20,    21,    81,    -1,    13,    20,    21,
      22,    28,    29,    30,    20,    21,    28,    29,    30,    -1,
      -1,    -1,    28,    29,    30
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,    12,    37,    38,    39,    40,    41,    47,
      48,    49,    53,     3,    49,     0,    39,    13,    13,    54,
      55,    13,    50,    51,    21,    27,    23,    26,    27,    23,
      26,    22,    42,    43,    49,    13,    20,    21,    28,    29,
      30,    56,    63,    64,    65,    66,    68,    69,    70,    71,
      72,    74,    76,    78,    55,    52,    62,    63,    51,    24,
      44,    22,    23,    13,    21,    63,    66,    31,    32,    33,
      28,    29,    14,    15,    34,    35,    73,    16,    17,    75,
      18,    77,    19,    79,     4,     6,     8,     9,    10,    11,
      25,    44,    45,    46,    47,    57,    58,    59,    60,    61,
      63,    65,    44,    43,    22,    63,    67,    22,    66,    66,
      66,    69,    69,    70,    72,    74,    76,    63,    21,    21,
      21,    25,    46,    26,    27,    22,    23,    63,    63,    46,
      63,    63,    22,    22,    63,    57,    59,    57,    26,     7,
      61,    58,    59,    22,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    36,    37,    38,    38,    39,    39,    40,    40,    41,
      41,    42,    42,    43,    44,    44,    45,    45,    46,    46,
      47,    47,    48,    49,    50,    50,    51,    52,    53,    54,
      54,    55,    55,    56,    57,    57,    58,    58,    59,    59,
      60,    60,    60,    60,    61,    61,    61,    61,    61,    61,
      61,    62,    63,    64,    64,    64,    65,    66,    66,    66,
      66,    67,    67,    68,    68,    68,    69,    69,    69,    69,
      70,    70,    70,    71,    72,    72,    73,    73,    73,    73,
      74,    74,    75,    75,    76,    76,    77,    78,    78,    79
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     5,     6,     1,
       1,     3,     1,     2,     3,     2,     2,     1,     1,     1,
       1,     1,     4,     1,     3,     1,     3,     1,     3,     3,
       1,     1,     3,     1,     1,     1,     5,     7,     1,     7,
       2,     1,     5,     8,     2,     1,     3,     1,     0,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     2,     3,
       4,     3,     1,     1,     1,     1,     1,     3,     3,     3,
       1,     3,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (ast, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, ast); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, std::unique_ptr<BaseAST> &ast)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (ast);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, std::unique_ptr<BaseAST> &ast)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, ast);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule, std::unique_ptr<BaseAST> &ast)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , ast);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, ast); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, std::unique_ptr<BaseAST> &ast)
{
  YYUSE (yyvaluep);
  YYUSE (ast);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (std::unique_ptr<BaseAST> &ast)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 58 "/root/compiler/src/frontend/sysy.y"
              {
    auto comp_unit = make_unique<CompUnitAST>();
    comp_unit->compitems = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast = move(comp_unit);
  }
#line 1465 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 3:
#line 66 "/root/compiler/src/frontend/sysy.y"
                       {
    CompItemsAST *ast = (CompItemsAST*)((yyvsp[-1].ast_val));
    ast->compitems.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1475 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 4:
#line 71 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new CompItemsAST();
    ast->compitems.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1485 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 5:
#line 79 "/root/compiler/src/frontend/sysy.y"
         {
    auto ast = new CompItemAST();
    ast->type = CompItemAST::DECL;
    ast->decl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1496 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 6:
#line 85 "/root/compiler/src/frontend/sysy.y"
            {
    auto ast = new CompItemAST();
    ast->type = CompItemAST::FUNC;
    ast->funcdef = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1507 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 7:
#line 104 "/root/compiler/src/frontend/sysy.y"
                                 {
    auto ast = new FuncDefAST();
    ast->type = FuncDefAST::NO_PARAMS;
    ast->func_type = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    ast->ident = *unique_ptr<string>((yyvsp[-3].str_val));
    ast->funcfparams = NULL;
    ast->block = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1521 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 8:
#line 113 "/root/compiler/src/frontend/sysy.y"
                                             {
    auto ast = new FuncDefAST();
    ast->type = FuncDefAST::HAS_PARAMS;
    ast->func_type = unique_ptr<BaseAST>((yyvsp[-5].ast_val));
    ast->ident = *unique_ptr<string>((yyvsp[-4].str_val));
    ast->funcfparams = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->block = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1535 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 9:
#line 126 "/root/compiler/src/frontend/sysy.y"
        {
    auto ast = new FuncTypeAST();
    ast->type = "int";
    (yyval.ast_val) = ast;
  }
#line 1545 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 10:
#line 131 "/root/compiler/src/frontend/sysy.y"
         {
    auto ast = new FuncTypeAST();
    ast->type = "void";
    (yyval.ast_val) = ast;
  }
#line 1555 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 11:
#line 139 "/root/compiler/src/frontend/sysy.y"
                               {
    FuncFParamsAST* ast = (FuncFParamsAST*)((yyvsp[-2].ast_val));
    ast->funcfparams.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1565 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 12:
#line 144 "/root/compiler/src/frontend/sysy.y"
               {
    auto ast = new FuncFParamsAST();
    ast->funcfparams.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1575 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 13:
#line 152 "/root/compiler/src/frontend/sysy.y"
                {
    auto ast = new FuncFParamAST();
    ast->btype = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->ident = *unique_ptr<string>((yyvsp[0].str_val));
    (yyval.ast_val) = ast;
  }
#line 1586 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 14:
#line 161 "/root/compiler/src/frontend/sysy.y"
                       {
    auto ast = new BlockAST();
    ast->blockitems = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1596 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 15:
#line 166 "/root/compiler/src/frontend/sysy.y"
            {
    auto ast = new BlockAST();
    ast->blockitems = NULL;
    (yyval.ast_val) = ast;
  }
#line 1606 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 16:
#line 174 "/root/compiler/src/frontend/sysy.y"
                         {
    BlockItemsAST *ast = (BlockItemsAST*)((yyvsp[-1].ast_val));
    ast->blockitems.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1616 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 17:
#line 179 "/root/compiler/src/frontend/sysy.y"
              {
    auto ast = new BlockItemsAST();
    ast->blockitems.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1626 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 18:
#line 187 "/root/compiler/src/frontend/sysy.y"
         {
    auto ast = new BlockItemAST();
    ast->type = BlockItemAST::DECL;
    ast->decl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1637 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 19:
#line 193 "/root/compiler/src/frontend/sysy.y"
         {
    auto ast = new BlockItemAST();
    ast->type = BlockItemAST::STMT;
    ast->stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1648 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 20:
#line 202 "/root/compiler/src/frontend/sysy.y"
              {
    auto ast = new DeclAST();
    ast->type = DeclAST::CON_DECL;
    ast->constdecl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1659 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 21:
#line 208 "/root/compiler/src/frontend/sysy.y"
            {
    auto ast = new DeclAST();
    ast->type = DeclAST::VAR_DECL;
    ast->vardecl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1670 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 22:
#line 217 "/root/compiler/src/frontend/sysy.y"
                              {
    auto ast = new ConstDeclAST();
    ast->btype = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->constdefs = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1681 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 23:
#line 226 "/root/compiler/src/frontend/sysy.y"
        {
    (yyval.str_val) = new string("int");
  }
#line 1689 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 24:
#line 232 "/root/compiler/src/frontend/sysy.y"
                           {
    ConstDefsAST *ast = (ConstDefsAST*)((yyvsp[-2].ast_val));
    ast->constdefs.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1699 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 25:
#line 237 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new ConstDefsAST();
    ast->constdefs.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1709 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 26:
#line 245 "/root/compiler/src/frontend/sysy.y"
                           {
    auto ast = new ConstDefAST();
    ast->ident = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->constinitval = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1720 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 27:
#line 254 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new ConstInitValAST();
    ast->constexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1730 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 28:
#line 262 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new VarDeclAST();
    ast->btype = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->vardefs = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1741 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 29:
#line 271 "/root/compiler/src/frontend/sysy.y"
                       {
    VarDefsAST *ast = (VarDefsAST*)((yyvsp[-2].ast_val));
    ast->vardefs.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1751 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 30:
#line 276 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new VarDefsAST();
    ast->vardefs.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1761 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 31:
#line 284 "/root/compiler/src/frontend/sysy.y"
          {
    auto ast = new VarDefAST();
    ast->type = VarDefAST::NO_VALUE;
    ast->ident = *unique_ptr<string>((yyvsp[0].str_val));
    (yyval.ast_val) = ast;
  }
#line 1772 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 32:
#line 290 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new VarDefAST();
    ast->type = VarDefAST::HAS_VALUE;
    ast->ident = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->initval = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1784 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 33:
#line 300 "/root/compiler/src/frontend/sysy.y"
        {
    auto ast = new InitValAST();
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1794 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 34:
#line 308 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new StmtAST();
    ast->type = StmtAST::OPENSTMT;
    ast->openstmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1805 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 35:
#line 314 "/root/compiler/src/frontend/sysy.y"
               {
    auto ast = new StmtAST();
    ast->type = StmtAST::CLOSEDSTMT;
    ast->closedstmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1816 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 36:
#line 323 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new OpenStmtAST();
    ast->type = OpenStmtAST::IF;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1828 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 37:
#line 330 "/root/compiler/src/frontend/sysy.y"
                                            {
    auto ast = new OpenStmtAST();
    ast->type = OpenStmtAST::IF_ELSE;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    ast->closedstmt = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->openstmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1841 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 38:
#line 341 "/root/compiler/src/frontend/sysy.y"
              {
    auto ast = new ClosedStmtAST();
    ast->type = ClosedStmtAST::NONIF;
    ast->nonifstmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1852 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 39:
#line 347 "/root/compiler/src/frontend/sysy.y"
                                              {
    auto ast = new ClosedStmtAST();
    ast->type = ClosedStmtAST::IF_ELSE;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    ast->closedstmt_if = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->closedstmt_else = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1865 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 40:
#line 358 "/root/compiler/src/frontend/sysy.y"
                 {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::STMT;
    ast->lessstmt = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1876 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 41:
#line 364 "/root/compiler/src/frontend/sysy.y"
          {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::BLOCK;
    ast->block = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1887 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 42:
#line 370 "/root/compiler/src/frontend/sysy.y"
                           {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::WHILE;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1899 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 43:
#line 377 "/root/compiler/src/frontend/sysy.y"
                                                 {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::FOR;
    ast->blockitem = unique_ptr<BaseAST>((yyvsp[-5].ast_val));
    ast->exp = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    ast->lessstmt = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1913 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 44:
#line 389 "/root/compiler/src/frontend/sysy.y"
               {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::RETURN;
    ast->ret = "return";
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1925 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 45:
#line 396 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::RETURN;
    ast->ret = "return";
    ast->exp = NULL;
    (yyval.ast_val) = ast;
  }
#line 1937 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 46:
#line 403 "/root/compiler/src/frontend/sysy.y"
                 {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::ASSIGN;
    ast->lval = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1949 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 47:
#line 410 "/root/compiler/src/frontend/sysy.y"
        {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::VOID;
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1960 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 48:
#line 416 "/root/compiler/src/frontend/sysy.y"
    {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::VOID;
    ast->exp = NULL;
    (yyval.ast_val) = ast;
  }
#line 1971 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 49:
#line 422 "/root/compiler/src/frontend/sysy.y"
          {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::BREAK;
    (yyval.ast_val) = ast;
  }
#line 1981 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 50:
#line 427 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::CONTINUE;
    (yyval.ast_val) = ast;
  }
#line 1991 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 51:
#line 435 "/root/compiler/src/frontend/sysy.y"
        {
    auto ast = new ConstExpAST();
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2001 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 52:
#line 443 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new ExpAST();
    ast->lorexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2011 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 53:
#line 451 "/root/compiler/src/frontend/sysy.y"
                {
    auto ast = new PrimaryExpAST();
    ast->type = PrimaryExpAST::EXP;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2022 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 54:
#line 457 "/root/compiler/src/frontend/sysy.y"
         {
    auto ast = new PrimaryExpAST();
    ast->type = PrimaryExpAST::LVAL;
    ast->lval = *unique_ptr<string>((yyvsp[0].str_val));
    (yyval.ast_val) = ast;
  }
#line 2033 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 55:
#line 463 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new PrimaryExpAST();
    ast->type = PrimaryExpAST::NUM;
    ast->number = (yyvsp[0].int_val);
    (yyval.ast_val) = ast;
  }
#line 2044 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 56:
#line 472 "/root/compiler/src/frontend/sysy.y"
          {
    (yyval.str_val) = (yyvsp[0].str_val);
  }
#line 2052 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 57:
#line 478 "/root/compiler/src/frontend/sysy.y"
               {
    auto ast = new UnaryExpAST();
    ast->type = UnaryExpAST::NAN;
    ast->primaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2063 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 58:
#line 484 "/root/compiler/src/frontend/sysy.y"
                     {
    auto ast = new UnaryExpAST();
    ast->type = UnaryExpAST::EXP;
    ast->unaryop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2075 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 59:
#line 491 "/root/compiler/src/frontend/sysy.y"
                  {
    auto ast = new UnaryExpAST();
    ast->type = UnaryExpAST::FUNC_NO_PARAMS;
    ast->ident = *unique_ptr<string>((yyvsp[-2].str_val));
    (yyval.ast_val) = ast;
  }
#line 2086 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 60:
#line 497 "/root/compiler/src/frontend/sysy.y"
                              {
    auto ast = new UnaryExpAST();
    ast->type = UnaryExpAST::FUNC_HAS_PARAMS;
    ast->ident = *unique_ptr<string>((yyvsp[-3].str_val));
    ast->funcrparams = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2098 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 61:
#line 507 "/root/compiler/src/frontend/sysy.y"
                        {
    FuncRParamsAST *ast = (FuncRParamsAST*)((yyvsp[-2].ast_val));
    ast->funcrparams.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 2108 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 62:
#line 512 "/root/compiler/src/frontend/sysy.y"
        {
    auto ast = new FuncRParamsAST();
    ast->funcrparams.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 2118 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 63:
#line 520 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string("+"); }
#line 2124 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 64:
#line 521 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string("-"); }
#line 2130 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 65:
#line 522 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string("!"); }
#line 2136 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 66:
#line 526 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::NAN;
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2147 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 67:
#line 532 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::EXP;
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->mulop = *unique_ptr<string>(new string("*"));
    (yyval.ast_val) = ast;
  }
#line 2160 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 68:
#line 540 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::EXP;
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->mulop = *unique_ptr<string>(new string("/"));
    (yyval.ast_val) = ast;
  }
#line 2173 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 69:
#line 548 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::EXP;
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->mulop = *unique_ptr<string>(new string("%"));
    (yyval.ast_val) = ast;
  }
#line 2186 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 70:
#line 559 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new AddExpAST();
    ast->type = AddExpAST::NAN;
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2197 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 71:
#line 565 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new AddExpAST();
    ast->type = AddExpAST::EXP;
    ast->addexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->addop = *unique_ptr<string>(new string("+"));
    (yyval.ast_val) = ast;
  }
#line 2210 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 72:
#line 573 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new AddExpAST();
    ast->type = AddExpAST::EXP;
    ast->addexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->addop = *unique_ptr<string>(new string("-"));
    (yyval.ast_val) = ast;
  }
#line 2223 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 73:
#line 584 "/root/compiler/src/frontend/sysy.y"
              {
    (yyval.int_val) = (yyvsp[0].int_val);
    // $$ = new string(to_string($1));
  }
#line 2232 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 74:
#line 591 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new RelExpAST();
    ast->type = RelExpAST::NAN;
    ast->addexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2243 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 75:
#line 597 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new RelExpAST();
    ast->type = RelExpAST::EXP;
    ast->relexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->relop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->addexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2256 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 76:
#line 608 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string("<"); }
#line 2262 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 77:
#line 609 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string(">"); }
#line 2268 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 78:
#line 610 "/root/compiler/src/frontend/sysy.y"
         { (yyval.str_val) = new string("<="); }
#line 2274 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 79:
#line 611 "/root/compiler/src/frontend/sysy.y"
         { (yyval.str_val) = new string(">="); }
#line 2280 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 80:
#line 615 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new EqExpAST();
    ast->type = EqExpAST::NAN;
    ast->relexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2291 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 81:
#line 621 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new EqExpAST();
    ast->type = EqExpAST::EXP;
    ast->eqexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->eqop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->relexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2304 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 82:
#line 632 "/root/compiler/src/frontend/sysy.y"
         { (yyval.str_val) = new string("=="); }
#line 2310 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 83:
#line 633 "/root/compiler/src/frontend/sysy.y"
          { (yyval.str_val) = new string("!="); }
#line 2316 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 84:
#line 637 "/root/compiler/src/frontend/sysy.y"
          {
    auto ast = new LAndExpAST();
    ast->type = LAndExpAST::NAN;
    ast->eqexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2327 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 85:
#line 643 "/root/compiler/src/frontend/sysy.y"
                         {
    auto ast = new LAndExpAST();
    ast->type = LAndExpAST::EXP;
    ast->landexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->landop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->eqexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2340 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 86:
#line 654 "/root/compiler/src/frontend/sysy.y"
          { (yyval.str_val) = new string("&&"); }
#line 2346 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 87:
#line 658 "/root/compiler/src/frontend/sysy.y"
            {
    auto ast = new LOrExpAST();
    ast->type = LOrExpAST::NAN;
    ast->landexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2357 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 88:
#line 664 "/root/compiler/src/frontend/sysy.y"
                         {
    auto ast = new LOrExpAST();
    ast->type = LOrExpAST::EXP;
    ast->lorexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->lorop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->landexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2370 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 89:
#line 675 "/root/compiler/src/frontend/sysy.y"
         { (yyval.str_val) = new string("||"); }
#line 2376 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;


#line 2380 "/root/compiler/build/frontend/sysy.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (ast, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (ast, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, ast);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, ast);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (ast, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, ast);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, ast);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 678 "/root/compiler/src/frontend/sysy.y"


// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}
