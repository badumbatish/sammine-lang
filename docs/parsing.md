# Parsing.md : High-level documentation describing parsing protocols and error handling

Hi there :) enjoy your stay
## Parsing protocol



## Error recovery

In principle, we say that if a Parse*() result is nullptr, then we haven't
committed to calling that Parse*() function yet. 

If a Parse*() result is non-null, then it can either parse successfully, or failed since its
commitment to parsing via that Parse*(). The error message is sent directly to a vector of error messages in parser.

If a Parse*() commits and at a certain time cannot match against a TokenType,
it will suppose that it has matched successfully against that token but reports an error.

There are two kind of Parse* that can be recoverable

### Statement Parsing
Simple statement parsing: if we encounter an error in matching a token, we exhaust the token stream until we hit a semicolon. Then we return that statement as if we have parsed that statement.
Of course we need to account for error reporting at appropriate places.

If statement parsing: if we encounter an error in matching a token, we exhaust the token stream until we hit a right curly bracket. Then we return that statement as if we have parsed that statement.
Of course we need to account for error reporting at appropriate places.

This means that for Simple statement, if a ParseExpr* commits and fails, it can push back to
error_msg vector and then promptly return nullptr. If simple statement has loop all over the function vector and
cannot have anything non-null, it exhausts until semicolon. Else if it has sth non-null. then it expects a semicolon, 
if it doesn't have it, then it errors and also exhausts until semicolon.


### Definition Parsing
Since a statement can be a simple statement which ends in a semicolon or an if
- Definition parsing
  - ClassDef: 
  - VarDef
  - FuncDef