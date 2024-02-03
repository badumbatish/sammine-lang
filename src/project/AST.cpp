#include "AST.h"

Value * NumberExprAST::Accept(AstVisitor* visitor) const  {
    return visitor->Visit(this);
}

Value * CallExprAST::Accept(AstVisitor* visitor) const  {
    return visitor->Visit(this);
}

Value * VariableExprAST::Accept(AstVisitor* visitor) const  {
    return visitor->Visit(this);
}

Value * BinaryExprAST::Accept(AstVisitor* visitor) const {
    return visitor->Visit(this);
}

Function * PrototypeAST::Accept(AstVisitor* visitor) const  {
    return visitor->Visit(this);
}

Function * FunctionAST::Accept(AstVisitor* visitor) const   {
    return visitor->Visit(this);
}


