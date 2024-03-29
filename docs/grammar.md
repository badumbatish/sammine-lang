# Grammar.md: A markdown file describing the grammar 

## Conventions
Terminal is Capitalized if multi-word, ALLCAPS if single word.

Non-terminal is bolden.

Non-terminal is not capitalized and uses _ as seperator (if any)


## Grammar


**program** ::= (**var_def** | **func_def**)* func_main? 

**typed_var** ::= ID : **type**

**type** ::= ID

**var_def** ::= LET **typed_var** ASSIGN **expr** SemiColon

**func_def** ::= FN ID LeftParen (**typed_var** (COMMA **typed_var**)*)?  RightParen -> **type** **block**


**def_block** ::= LeftCurly **stmt*** **return_stmt?** RightCurly

**block** ::= LeftCurly **stmt***  RightCurly

**stmt** ::= **simple_stmt** <br>
&emsp; | IF **expr** COLON **block** [ ELSE IF ]

**simple_stmt** ::= (**expr** | **assign_expr**) SemiColon

**return_stmt** ::= RETURN **expr** SemiColon

**assign_expr** ::= **target** ASSIGN **expr**

**expr** ::= **cexpr** <br>
&emsp; | NOT **expr** <br>
&emsp; | **expr** [AND | OR] **expr**

**cexpr** ::= ID <br>
&emsp; | StringLiteral <br>
&emsp; | LeftParen **expr** RightParen <br>
&emsp; | **member_expr** <br>
&emsp; | **member_expr** LeftParen (**expr** (COMMA **expr**)*)? RightParen <br>
&emsp; | **index_expr** <br>
&emsp; | ID LeftParen (**expr** (COMMA **expr**)*)? RightParen <br>
&emsp; | **cexpr** **bin_op** **cexpr** <br>
&emsp; | **un_op** **cexpr** <br>
&emsp; | **cexpr** **un_op** <br>

**member_expr** ::= **cexpr** DOT ID 

**index_expr** ::= LeftBracket  **expr** RightBracket

**bin_op** ::= + | - | * | / | % | += | -= <br>
&emsp; | *= | /= | && | DoublePipe | SingularPipe | <br>
&emsp; | ^ | << | >> | == | < | > | <= | >= | ** | <br>
&emsp; | /_ | /^ 


**target** ::= ID <br>
&emsp; | **member_expr** <br>
&emsp; | **index_expr** 

**un_op** ::= ++ | -- | -


