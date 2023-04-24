// #pragma once

// #include "errmsgs.h"
// #include "lexer.h"
// #include "token.h"
// #include "type.h"

// // Expressions
// struct Expr;
// struct LitExpr;
// struct BinaryExpr;
// struct UnaryExpr;

// // Ast and Symbol table
// struct Ast;
// struct AstImport;
// struct AstGlVar;
// struct AstStruct;
// struct AstEnum;
// struct AstFn;

// /*
//  * Expressions
//  */

// enum class BinaryOpType : u8
// {
//     // TODO: add all
//     add,     // +
//     sub,     // -
//     mult,    // *
//     div,     // '/'
//     eqleql,  // ==
//     greater, // >
//     greql,   // >=
//     less,    // <
//     leseql,  // <=
//              // TODO: more operators
// };

// enum class UnaryOpType : u8
// {
//     logical_negate,   // '!'
//     numerical_negate, // -
//     new_opr,          // new (only allowed as expressions)
//     delete_opr,       // delete (only allowed as stmts)
// };

// struct LitExpr
// {
//     // LiteralExprType type;
//     TknIdx val_idx;

//     LitExpr(TknIdx i) : val_idx(i)
//     {
//     }

//     ~LitExpr() = default;
// };

// struct UnaryExpr
// {
//     UnaryOpType op;
//     Expr *expr;

//     UnaryExpr(UnaryOpType o, Expr *expr) : op(o), expr(expr)
//     {
//     }

//     ~UnaryExpr() = default;
// };

// struct BinaryExpr
// {
//     BinaryOpType op;
//     Expr *left, *right;

//     BinaryExpr(BinaryOpType o, Expr *l, Expr *r) : op(o), left(l), right(r)
//     {
//     }

//     ~BinaryExpr() = default;
// };

// struct ArrayExpr
// {
//     Type type;
//     usize size;
//     ArrayList<Expr> expressions;
// };

// struct FnCallExpr
// {
//     TknIdx id;
//     Type return_type;
//     ArrayList<Expr> input_params;
//     // NOTE(5717): must be non-void return type
// };

// enum class ExprType : u8
// {
//     Literal,
//     ArraySub,
//     UnaryExpr,
//     ArrayLiteral,
//     BinaryExpr,
//     FuncCallNonVoid,
// };

// struct Expr
// {
//     ExprIdx idx;
//     ExprType type;

//     Expr(ExprIdx i, ExprType t) : idx(i), type(t)
//     {
//     }

//     ~Expr() = default;
// };

// // NOTE: array[i]
// struct ArraySubExpr
// {
//     TknIdx id; // array id
//     Expr i;

//     ArraySubExpr(TknIdx id, Expr e) : id(id), i(e)
//     {
//     }

//     ~ArraySubExpr() = default;
// };

// // NOTE(5717): used as func params, struct params or variables
// struct VarDef
// {
//     TknIdx id;
//     Type type;

//     VarDef(TknIdx id, Type t) : id(id), type(t)
//     {
//     }

//     ~VarDef() = default;
// };
// /*
//  * Abstract Syntax tree
//  */
// struct Ast
// {
//     // Globals
//     ArrayList<AstImport> imports;
//     ArrayList<AstGlVar> gl_variables;
//     ArrayList<AstStruct> structs;
//     ArrayList<AstEnum> enums;
//     ArrayList<AstFn> functions;
//     // Expressions
//     ArrayList<Expr> expressions;
//     ArrayList<LitExpr> literals;
//     ArrayList<BinaryExpr> bin_exprs;
//     ArrayList<UnaryExpr> unary_exprs;
//     ArrayList<ArrayExpr> arr_exprs;
//     ArrayList<FnCallExpr> fn_exprs;
//     // types
//     SymbolTable stable;
// };

// struct AstImport
// {

//     // 0  1  2     34    56
//     // import "std/io" as io;
//     //         ^^^^^^val   ^^id
//     // index is the index of cstr token
//     // It requires a single cstr Token for path
//     // the path must be known at compile time
//     TknIdx alias_id;
//     TknIdx import_str;
//     bool aliased;

//     AstImport(uint import_str) : import_str(import_str), aliased(false)
//     {
//     }

//     AstImport(TknIdx alias_id, TknIdx import_str)
//         : alias_id(alias_id), import_str(import_str), aliased(true)
//     {
//     }

//     ~AstImport() = default;
// };

// struct AstGlVar
// {
//     TknIdx id_idx;
//     Type type{};
//     ExprIdx value;

//     AstGlVar(TknIdx i, Type t, ExprIdx e) : id_idx(i), type(t), value(e)
//     {
//     }

//     ~AstGlVar() = default;
// };

// struct AstStruct
// {
//     TknIdx id;
//     ArrayList<VarDef> members;
// };

// struct AstEnum
// {
//     TknIdx id;
//     ArrayList<TknIdx> members;
// };

// struct AstBlock
// {
//     // TODO: vector of statements
// };

// struct AstFn
// {
//     TknIdx id;
//     Type return_type;
//     AstBlock block;
//     ArrayList<VarDef> params;

//     AstFn(TknIdx i, Type r, AstBlock b, ArrayList<VarDef> p)
//         : id(i), return_type(r), block(b), params(p)
//     {
//     }

//     ~AstFn() = default;
// };

// class Parser
// {
//     // starting point
//     u8 parse_starter();

//     // global stmts
//     u8 parse_import();
//     u8 parse_function();
//     u8 parse_gl_var();
//     u8 parse_struct();
//     u8 parse_enum();

//     // outline function specific
//     u8 parse_block();
//     u8 parse_fn_params();
//     u8 parse_fn_params_types();

//     // statements (inner) function specific
//     u8 parse_var_def();
//     u8 parse_var_decl();
//     u8 parse_while_loop();
//     u8 parse_voidfunction_call();
//     u8 parse_for_loop();
//     u8 parse_if_else();
//     u8 parse_mutate_var();
//     u8 parse_defer_stmt();
//     u8 parse_switch_stmt();
//     u8 parse_ignore_vars();

//     // Types
//     Type parse_type();
//     Type parse_fn_type();

//     // expressions
//     ExprIdx parse_expr(const TknType, bool *);
//     BinaryExpr parse_bin_expr(const TknType, bool *);
//     UnaryExpr parse_unary_expr(const TknType, bool *);
//     Expr parse_grouping_expr(const TknType, bool *);
//     ArrayExpr parse_array_literal_expr(const TknType, bool *);
//     LitExpr parse_literal_expr(const TknType, bool *);
//     FnCallExpr parse_void_fn_call_expr(const TknType, bool *);

//     // utils
//     Token current() const;
//     Token past() const;
//     Token peek() const;
//     void advance();

//     // err msgs
//     u8 parse_error_expect_token(const TknType);
//     PrsErr convert_tkn_type_to_parse_error(const TknType);
//     u8 parser_error(const PrsErr);
//     cstr parser_error_msg(const PrsErr);
//     cstr parser_error_advice(const PrsErr);
//     u8 parse_error_use_global_err();

//     // add expressions
//     ExprIdx add_literal_expr(const LitExpr);
//     ExprIdx add_binary_expr(const BinaryExpr);
//     ExprIdx add_unary_expr(const UnaryExpr);
//     ExprIdx add_array_literal_expr(const ArrayExpr);
//     ExprIdx add_array_sub_literal_expr(const ArraySubExpr);
//     ExprIdx add_nonvoid_fn_call_expr(const FnCallExpr);
//     // add globals
//     // NOTE: not needed for now

//     // PRIVATE members
//     // not owned (weak ptr)
//     const file_t *file;
//     const ArrayList<Token> *tokens;
//     //
//     PrsErr error;
//     uint idx; // of tokens
//     uint ExprIndex, LitIdx, BinIdx, UnaryIdx, ArrayExprIdx, ArraySubIdx, FnCallIdx;

//   public:
//     Ast *ast;
//     // Parser exports
//     Parser(const file_t *, const Lexer *);
//     ~Parser();
//     u8 parse_lexer();
// }; // Parser
