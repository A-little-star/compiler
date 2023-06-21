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
    IDENT = 267,
    LTOP = 268,
    GTOP = 269,
    EQOP = 270,
    NEQOP = 271,
    ANDOP = 272,
    OROP = 273,
    INT_CONST = 274
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

#line 171 "/root/compiler/build/frontend/sysy.tab.cpp"

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
typedef yytype_int8 yy_state_t;

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
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   165

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  76
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  125

#define YYUNDEFTOK  2
#define YYMAXUTOK   274


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
       2,     2,     2,    29,     2,     2,     2,    32,     2,     2,
      20,    21,    30,    27,    25,    28,     2,    31,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    24,
      33,    26,    34,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    22,     2,    23,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    58,    58,    76,    87,    95,   100,   108,   113,   121,
     127,   136,   142,   151,   160,   166,   171,   179,   188,   196,
     205,   210,   218,   224,   234,   242,   248,   257,   264,   275,
     281,   292,   298,   304,   311,   323,   330,   337,   344,   350,
     356,   361,   369,   377,   385,   391,   397,   406,   412,   418,
     428,   429,   430,   434,   440,   448,   456,   467,   473,   481,
     492,   499,   505,   516,   517,   518,   519,   523,   529,   540,
     541,   545,   551,   562,   566,   572,   583
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "RETURN", "CONST", "IF", "ELSE",
  "WHILE", "BREAK", "CONTINUE", "FOR", "IDENT", "LTOP", "GTOP", "EQOP",
  "NEQOP", "ANDOP", "OROP", "INT_CONST", "'('", "')'", "'{'", "'}'", "';'",
  "','", "'='", "'+'", "'-'", "'!'", "'*'", "'/'", "'%'", "'<'", "'>'",
  "$accept", "CompUnit", "FuncDef", "FuncType", "Block", "BlockItems",
  "BlockItem", "Decl", "ConstDecl", "BType", "ConstDefs", "ConstDef",
  "ConstInitVal", "VarDecl", "VarDefs", "VarDef", "InitVal", "Stmt",
  "OpenStmt", "ClosedStmt", "NonIfStmt", "LessStmt", "ConstExp", "Exp",
  "PrimaryExp", "LVal", "UnaryExp", "UnaryOp", "MulExp", "AddExp",
  "Number", "RelExp", "RelOp", "EqExp", "EqOp", "LAndExp", "LAndOp",
  "LOrExp", "LOrOp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      40,    41,   123,   125,    59,    44,    61,    43,    45,    33,
      42,    47,    37,    60,    62
};
# endif

#define YYPACT_NINF (-102)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      12,  -102,    19,  -102,     9,  -102,     3,     8,    13,    28,
    -102,  -102,    65,    38,    40,    41,  -102,  -102,    42,  -102,
    -102,    65,  -102,  -102,  -102,  -102,  -102,    60,  -102,  -102,
    -102,    37,  -102,  -102,  -102,  -102,  -102,    43,  -102,  -102,
      47,  -102,    65,   -23,   -16,  -102,    11,    -2,    57,    63,
    -102,  -102,    74,    65,    65,    96,    54,  -102,  -102,    64,
      -8,  -102,  -102,    65,  -102,    65,    65,    65,    65,    65,
    -102,  -102,  -102,  -102,    65,  -102,  -102,    65,  -102,    65,
    -102,    65,    69,    29,  -102,    77,    82,    65,  -102,    65,
    -102,    37,  -102,  -102,  -102,  -102,   -23,   -23,   -16,    11,
      -2,    57,    65,  -102,    74,   122,   122,    87,  -102,  -102,
    -102,  -102,  -102,  -102,  -102,  -102,   105,  -102,   136,   122,
      92,  -102,  -102,   122,  -102
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     0,     2,     0,     1,     0,     0,     0,    39,
       3,    14,    36,     0,     0,     0,    40,    41,     0,    47,
      60,     0,     6,    50,    51,    52,    32,    39,     8,     9,
      11,     0,    12,    10,    25,    26,    29,     0,    38,    48,
      45,    53,     0,    57,    61,    46,    67,    71,    74,    43,
      35,    45,     0,     0,     0,    39,     0,     5,     7,    22,
       0,    21,    31,     0,    49,     0,     0,     0,     0,     0,
      65,    66,    63,    64,     0,    69,    70,     0,    73,     0,
      76,     0,     0,     0,    16,     0,     0,     0,    44,     0,
      19,     0,    37,    54,    55,    56,    58,    59,    62,    68,
      72,    75,     0,    13,     0,    39,    39,     0,    23,    24,
      20,    17,    18,    42,    15,    27,    26,    33,    39,    39,
       0,    28,    30,    39,    34
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -102,  -102,  -102,  -102,   109,  -102,   -25,  -102,  -102,   106,
    -102,    16,  -102,  -102,  -102,    30,  -102,  -101,    10,   -99,
    -102,     4,  -102,   -11,  -102,    -9,   -39,  -102,   -10,    53,
    -102,    58,  -102,    59,  -102,    55,  -102,  -102,  -102
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     4,    26,    27,    28,    29,    30,    31,
      83,    84,   111,    32,    60,    61,   108,    33,    34,    35,
      36,    37,   112,    38,    39,    51,    41,    42,    43,    44,
      45,    46,    74,    47,    77,    48,    79,    49,    81
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      40,    50,    58,    64,   115,   117,   116,    65,    66,    67,
      56,    68,    69,    75,    76,     1,    90,    91,    40,     5,
     122,     6,   124,     7,    70,    71,    93,    94,    95,     8,
      87,    11,    12,    13,    14,     9,    15,    16,    17,    18,
      19,    11,    85,    86,    72,    73,    40,    20,    21,    59,
       9,    22,    92,   103,   104,    23,    24,    25,    96,    97,
      53,    54,    55,    11,    12,    13,    14,    62,    15,    16,
      17,    18,    19,    63,    78,    88,   107,    19,   109,    20,
      21,    80,     9,    57,    20,    21,    82,    23,    24,    25,
      89,   113,    23,    24,    25,   102,    40,    40,   105,    11,
      12,    13,    14,   106,    15,    16,    17,    18,    19,    40,
      40,   118,   119,   123,    40,    20,    21,    10,     9,    52,
     114,   110,   120,    23,    24,    25,    12,    98,    14,   121,
      15,    16,    17,    18,    19,    99,   101,     0,   100,     0,
      12,    20,    21,     0,     9,    16,    17,     0,    19,    23,
      24,    25,     0,     0,     0,    20,    21,     0,     0,     0,
       0,     0,     0,    23,    24,    25
};

static const yytype_int8 yycheck[] =
{
       9,    12,    27,    42,   105,   106,   105,    30,    31,    32,
      21,    27,    28,    15,    16,     3,    24,    25,    27,     0,
     119,    12,   123,    20,    13,    14,    65,    66,    67,    21,
      55,     3,     4,     5,     6,    22,     8,     9,    10,    11,
      12,     3,    53,    54,    33,    34,    55,    19,    20,    12,
      22,    23,    63,    24,    25,    27,    28,    29,    68,    69,
      20,    20,    20,     3,     4,     5,     6,    24,     8,     9,
      10,    11,    12,    26,    17,    21,    87,    12,    89,    19,
      20,    18,    22,    23,    19,    20,    12,    27,    28,    29,
      26,   102,    27,    28,    29,    26,   105,   106,    21,     3,
       4,     5,     6,    21,     8,     9,    10,    11,    12,   118,
     119,    24,     7,    21,   123,    19,    20,     8,    22,    13,
     104,    91,   118,    27,    28,    29,     4,    74,     6,   119,
       8,     9,    10,    11,    12,    77,    81,    -1,    79,    -1,
       4,    19,    20,    -1,    22,     9,    10,    -1,    12,    27,
      28,    29,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    28,    29
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    36,    37,    38,     0,    12,    20,    21,    22,
      39,     3,     4,     5,     6,     8,     9,    10,    11,    12,
      19,    20,    23,    27,    28,    29,    39,    40,    41,    42,
      43,    44,    48,    52,    53,    54,    55,    56,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    68,    70,    72,
      58,    60,    44,    20,    20,    20,    58,    23,    41,    12,
      49,    50,    24,    26,    61,    30,    31,    32,    27,    28,
      13,    14,    33,    34,    67,    15,    16,    69,    17,    71,
      18,    73,    12,    45,    46,    58,    58,    41,    21,    26,
      24,    25,    58,    61,    61,    61,    63,    63,    64,    66,
      68,    70,    26,    24,    25,    21,    21,    58,    51,    58,
      50,    47,    57,    58,    46,    52,    54,    52,    24,     7,
      56,    53,    54,    21,    52
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    35,    36,    37,    38,    39,    39,    40,    40,    41,
      41,    42,    42,    43,    44,    45,    45,    46,    47,    48,
      49,    49,    50,    50,    51,    52,    52,    53,    53,    54,
      54,    55,    55,    55,    55,    56,    56,    56,    56,    56,
      56,    56,    57,    58,    59,    59,    59,    60,    61,    61,
      62,    62,    62,    63,    63,    63,    63,    64,    64,    64,
      65,    66,    66,    67,    67,    67,    67,    68,    68,    69,
      69,    70,    70,    71,    72,    72,    73
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     5,     1,     3,     2,     2,     1,     1,
       1,     1,     1,     4,     1,     3,     1,     3,     1,     3,
       3,     1,     1,     3,     1,     1,     1,     5,     7,     1,
       7,     2,     1,     5,     8,     2,     1,     3,     1,     0,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     3,     3,     3,     1,     3,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     1,     3,     1
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
    comp_unit->func_def = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast = move(comp_unit);
  }
#line 1448 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 3:
#line 76 "/root/compiler/src/frontend/sysy.y"
                                 {
    auto ast = new FuncDefAST();
    ast->func_type = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    ast->ident = *unique_ptr<string>((yyvsp[-3].str_val));
    ast->block = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1460 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 4:
#line 87 "/root/compiler/src/frontend/sysy.y"
        {
    auto ast = new FuncTypeAST();
    ast->type = "int";
    (yyval.ast_val) = ast;
  }
#line 1470 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 5:
#line 95 "/root/compiler/src/frontend/sysy.y"
                       {
    auto ast = new BlockAST();
    ast->blockitems = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1480 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 6:
#line 100 "/root/compiler/src/frontend/sysy.y"
            {
    auto ast = new BlockAST();
    ast->blockitems = NULL;
    (yyval.ast_val) = ast;
  }
#line 1490 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 7:
#line 108 "/root/compiler/src/frontend/sysy.y"
                         {
    BlockItemsAST *ast = (BlockItemsAST*)((yyvsp[-1].ast_val));
    ast->blockitems.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1500 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 8:
#line 113 "/root/compiler/src/frontend/sysy.y"
              {
    auto ast = new BlockItemsAST();
    ast->blockitems.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1510 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 9:
#line 121 "/root/compiler/src/frontend/sysy.y"
         {
    auto ast = new BlockItemAST();
    ast->type = BlockItemAST::DECL;
    ast->decl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1521 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 10:
#line 127 "/root/compiler/src/frontend/sysy.y"
         {
    auto ast = new BlockItemAST();
    ast->type = BlockItemAST::STMT;
    ast->stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1532 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 11:
#line 136 "/root/compiler/src/frontend/sysy.y"
              {
    auto ast = new DeclAST();
    ast->type = DeclAST::CON_DECL;
    ast->constdecl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1543 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 12:
#line 142 "/root/compiler/src/frontend/sysy.y"
            {
    auto ast = new DeclAST();
    ast->type = DeclAST::VAR_DECL;
    ast->vardecl = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1554 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 13:
#line 151 "/root/compiler/src/frontend/sysy.y"
                              {
    auto ast = new ConstDeclAST();
    ast->btype = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->constdefs = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1565 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 14:
#line 160 "/root/compiler/src/frontend/sysy.y"
        {
    (yyval.str_val) = new string("int");
  }
#line 1573 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 15:
#line 166 "/root/compiler/src/frontend/sysy.y"
                           {
    ConstDefsAST *ast = (ConstDefsAST*)((yyvsp[-2].ast_val));
    ast->constdefs.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1583 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 16:
#line 171 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new ConstDefsAST();
    ast->constdefs.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1593 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 17:
#line 179 "/root/compiler/src/frontend/sysy.y"
                           {
    auto ast = new ConstDefAST();
    ast->ident = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->constinitval = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1604 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 18:
#line 188 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new ConstInitValAST();
    ast->constexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1614 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 19:
#line 196 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new VarDeclAST();
    ast->btype = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->vardefs = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1625 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 20:
#line 205 "/root/compiler/src/frontend/sysy.y"
                       {
    VarDefsAST *ast = (VarDefsAST*)((yyvsp[-2].ast_val));
    ast->vardefs.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1635 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 21:
#line 210 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new VarDefsAST();
    ast->vardefs.emplace_back(unique_ptr<BaseAST>((yyvsp[0].ast_val)));
    (yyval.ast_val) = ast;
  }
#line 1645 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 22:
#line 218 "/root/compiler/src/frontend/sysy.y"
          {
    auto ast = new VarDefAST();
    ast->type = VarDefAST::NO_VALUE;
    ast->ident = *unique_ptr<string>((yyvsp[0].str_val));
    (yyval.ast_val) = ast;
  }
#line 1656 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 23:
#line 224 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new VarDefAST();
    ast->type = VarDefAST::HAS_VALUE;
    ast->ident = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->initval = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1668 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 24:
#line 234 "/root/compiler/src/frontend/sysy.y"
        {
    auto ast = new InitValAST();
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1678 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 25:
#line 242 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new StmtAST();
    ast->type = StmtAST::OPENSTMT;
    ast->openstmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1689 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 26:
#line 248 "/root/compiler/src/frontend/sysy.y"
               {
    auto ast = new StmtAST();
    ast->type = StmtAST::CLOSEDSTMT;
    ast->closedstmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1700 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 27:
#line 257 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new OpenStmtAST();
    ast->type = OpenStmtAST::IF;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1712 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 28:
#line 264 "/root/compiler/src/frontend/sysy.y"
                                            {
    auto ast = new OpenStmtAST();
    ast->type = OpenStmtAST::IF_ELSE;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    ast->closedstmt = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->openstmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1725 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 29:
#line 275 "/root/compiler/src/frontend/sysy.y"
              {
    auto ast = new ClosedStmtAST();
    ast->type = ClosedStmtAST::NONIF;
    ast->nonifstmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1736 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 30:
#line 281 "/root/compiler/src/frontend/sysy.y"
                                              {
    auto ast = new ClosedStmtAST();
    ast->type = ClosedStmtAST::IF_ELSE;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    ast->closedstmt_if = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->closedstmt_else = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1749 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 31:
#line 292 "/root/compiler/src/frontend/sysy.y"
                 {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::STMT;
    ast->lessstmt = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1760 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 32:
#line 298 "/root/compiler/src/frontend/sysy.y"
          {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::BLOCK;
    ast->block = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1771 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 33:
#line 304 "/root/compiler/src/frontend/sysy.y"
                           {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::WHILE;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1783 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 34:
#line 311 "/root/compiler/src/frontend/sysy.y"
                                                 {
    auto ast = new NonIfStmtAST();
    ast->type = NonIfStmtAST::FOR;
    ast->blockitem = unique_ptr<BaseAST>((yyvsp[-5].ast_val));
    ast->exp = unique_ptr<BaseAST>((yyvsp[-4].ast_val));
    ast->lessstmt = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->stmt = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1797 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 35:
#line 323 "/root/compiler/src/frontend/sysy.y"
               {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::RETURN;
    ast->ret = "return";
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1809 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 36:
#line 330 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::RETURN;
    ast->ret = "return";
    ast->exp = NULL;
    (yyval.ast_val) = ast;
  }
#line 1821 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 37:
#line 337 "/root/compiler/src/frontend/sysy.y"
                 {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::ASSIGN;
    ast->lval = *unique_ptr<string>((yyvsp[-2].str_val));
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1833 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 38:
#line 344 "/root/compiler/src/frontend/sysy.y"
        {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::VOID;
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1844 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 39:
#line 350 "/root/compiler/src/frontend/sysy.y"
    {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::VOID;
    ast->exp = NULL;
    (yyval.ast_val) = ast;
  }
#line 1855 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 40:
#line 356 "/root/compiler/src/frontend/sysy.y"
          {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::BREAK;
    (yyval.ast_val) = ast;
  }
#line 1865 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 41:
#line 361 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new LessStmtAST();
    ast->type = LessStmtAST::CONTINUE;
    (yyval.ast_val) = ast;
  }
#line 1875 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 42:
#line 369 "/root/compiler/src/frontend/sysy.y"
        {
    auto ast = new ConstExpAST();
    ast->exp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1885 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 43:
#line 377 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new ExpAST();
    ast->lorexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1895 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 44:
#line 385 "/root/compiler/src/frontend/sysy.y"
                {
    auto ast = new PrimaryExpAST();
    ast->type = PrimaryExpAST::EXP;
    ast->exp = unique_ptr<BaseAST>((yyvsp[-1].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1906 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 45:
#line 391 "/root/compiler/src/frontend/sysy.y"
         {
    auto ast = new PrimaryExpAST();
    ast->type = PrimaryExpAST::LVAL;
    ast->lval = *unique_ptr<string>((yyvsp[0].str_val));
    (yyval.ast_val) = ast;
  }
#line 1917 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 46:
#line 397 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new PrimaryExpAST();
    ast->type = PrimaryExpAST::NUM;
    ast->number = (yyvsp[0].int_val);
    (yyval.ast_val) = ast;
  }
#line 1928 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 47:
#line 406 "/root/compiler/src/frontend/sysy.y"
          {
    (yyval.str_val) = (yyvsp[0].str_val);
  }
#line 1936 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 48:
#line 412 "/root/compiler/src/frontend/sysy.y"
               {
    auto ast = new UnaryExpAST();
    ast->type = UnaryExpAST::NAN;
    ast->primaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1947 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 49:
#line 418 "/root/compiler/src/frontend/sysy.y"
                     {
    auto ast = new UnaryExpAST();
    ast->type = UnaryExpAST::EXP;
    ast->unaryop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1959 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 50:
#line 428 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string("+"); }
#line 1965 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 51:
#line 429 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string("-"); }
#line 1971 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 52:
#line 430 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string("!"); }
#line 1977 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 53:
#line 434 "/root/compiler/src/frontend/sysy.y"
             {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::NAN;
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 1988 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 54:
#line 440 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::EXP;
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->mulop = *unique_ptr<string>(new string("*"));
    (yyval.ast_val) = ast;
  }
#line 2001 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 55:
#line 448 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::EXP;
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->mulop = *unique_ptr<string>(new string("/"));
    (yyval.ast_val) = ast;
  }
#line 2014 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 56:
#line 456 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new MulExpAST();
    ast->type = MulExpAST::EXP;
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->unaryexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->mulop = *unique_ptr<string>(new string("%"));
    (yyval.ast_val) = ast;
  }
#line 2027 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 57:
#line 467 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new AddExpAST();
    ast->type = AddExpAST::NAN;
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2038 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 58:
#line 473 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new AddExpAST();
    ast->type = AddExpAST::EXP;
    ast->addexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->addop = *unique_ptr<string>(new string("+"));
    (yyval.ast_val) = ast;
  }
#line 2051 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 59:
#line 481 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new AddExpAST();
    ast->type = AddExpAST::EXP;
    ast->addexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->mulexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    ast->addop = *unique_ptr<string>(new string("-"));
    (yyval.ast_val) = ast;
  }
#line 2064 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 60:
#line 492 "/root/compiler/src/frontend/sysy.y"
              {
    (yyval.int_val) = (yyvsp[0].int_val);
    // $$ = new string(to_string($1));
  }
#line 2073 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 61:
#line 499 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new RelExpAST();
    ast->type = RelExpAST::NAN;
    ast->addexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2084 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 62:
#line 505 "/root/compiler/src/frontend/sysy.y"
                        {
    auto ast = new RelExpAST();
    ast->type = RelExpAST::EXP;
    ast->relexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->relop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->addexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2097 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 63:
#line 516 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string("<"); }
#line 2103 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 64:
#line 517 "/root/compiler/src/frontend/sysy.y"
        { (yyval.str_val) = new string(">"); }
#line 2109 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 65:
#line 518 "/root/compiler/src/frontend/sysy.y"
         { (yyval.str_val) = new string("<="); }
#line 2115 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 66:
#line 519 "/root/compiler/src/frontend/sysy.y"
         { (yyval.str_val) = new string(">="); }
#line 2121 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 67:
#line 523 "/root/compiler/src/frontend/sysy.y"
           {
    auto ast = new EqExpAST();
    ast->type = EqExpAST::NAN;
    ast->relexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2132 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 68:
#line 529 "/root/compiler/src/frontend/sysy.y"
                      {
    auto ast = new EqExpAST();
    ast->type = EqExpAST::EXP;
    ast->eqexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->eqop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->relexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2145 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 69:
#line 540 "/root/compiler/src/frontend/sysy.y"
         { (yyval.str_val) = new string("=="); }
#line 2151 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 70:
#line 541 "/root/compiler/src/frontend/sysy.y"
          { (yyval.str_val) = new string("!="); }
#line 2157 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 71:
#line 545 "/root/compiler/src/frontend/sysy.y"
          {
    auto ast = new LAndExpAST();
    ast->type = LAndExpAST::NAN;
    ast->eqexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2168 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 72:
#line 551 "/root/compiler/src/frontend/sysy.y"
                         {
    auto ast = new LAndExpAST();
    ast->type = LAndExpAST::EXP;
    ast->landexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->landop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->eqexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2181 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 73:
#line 562 "/root/compiler/src/frontend/sysy.y"
          { (yyval.str_val) = new string("&&"); }
#line 2187 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 74:
#line 566 "/root/compiler/src/frontend/sysy.y"
            {
    auto ast = new LOrExpAST();
    ast->type = LOrExpAST::NAN;
    ast->landexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2198 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 75:
#line 572 "/root/compiler/src/frontend/sysy.y"
                         {
    auto ast = new LOrExpAST();
    ast->type = LOrExpAST::EXP;
    ast->lorexp = unique_ptr<BaseAST>((yyvsp[-2].ast_val));
    ast->lorop = *unique_ptr<string>((yyvsp[-1].str_val));
    ast->landexp = unique_ptr<BaseAST>((yyvsp[0].ast_val));
    (yyval.ast_val) = ast;
  }
#line 2211 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;

  case 76:
#line 583 "/root/compiler/src/frontend/sysy.y"
         { (yyval.str_val) = new string("||"); }
#line 2217 "/root/compiler/build/frontend/sysy.tab.cpp"
    break;


#line 2221 "/root/compiler/build/frontend/sysy.tab.cpp"

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
#line 586 "/root/compiler/src/frontend/sysy.y"


// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<BaseAST> &ast, const char *s) {
  cerr << "error: " << s << endl;
}
