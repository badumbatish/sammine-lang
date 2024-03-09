//! \file

# Grammar.md: A markdown file describing the grammar 

## Conventions
Terminal is Capitalized if multi-word, ALLCAPS if single word

Non-terminal is not capitalized and uses _ as seperator (if any)


## Grammar
program ::= (var_def | func_def)* main_func_def?

typed_var ::= ID : type
type ::= ID
var_def ::= typed_var ASSIGN expr SemiColon
func_def ::= FUNC ID LeftParen (typed_var (COMMA typed_var)*)?  RightParen -> type def_block


def_block ::= LeftCurly stmt* return_expr RightCurly
block ::= LeftCurly stmt*  RightCurly
stmt ::= simple_stmt SemiColon | 
    | IF expr COLON block [ ELSE IF ]

simple_stmt ::= expr | assign_expr
return_expr ::= return expr
assign_expr ::= target ASSIGN expr

expr ::= cexpr
    | NOT expr
    | expr [AND | OR] expr

cexpr ::= ID 
    | StringLiteral 
    | LeftParen expr RightParen
    | member_expr 
    | member_expr LeftParen (expr (COMMA expr)*)? RightParen
    | index_expr
    | ID LeftParen (expr (COMMA expr)*)? RightParen
    | cexpr bin_op cexpr
    | un_op cexpr
    | cexpr un_op

member_expr ::= cexpr DOT ID
index_expr ::= LeftBracket  expr RightBracket
bin_op ::= + | - | * | / | % | += | -= 
    | *= | /= | && | DoublePipe | SingularPipe |
    | ^ | << | >> | == | < | > | <= | >= | ** |
    | /_ | /^ 


target ::= ID | member_expr | index_expr 
un_op ::= ++ | -- | -
