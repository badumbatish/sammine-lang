#include "AST.h"

Value * NumberExprAST::Accept(std::shared_ptr<AstVisitor> visitor) const  {
    return visitor->VisitNumberExprAST(this);
}

Value * CallExprAST::Accept(std::shared_ptr<AstVisitor> visitor) const  {
    return visitor->VisitCallExprAST(this);
}

Value * VariableExprAST::Accept(std::shared_ptr<AstVisitor> visitor) const  {
    return visitor->VisitVariableExprAST(this);
}

Value * BinaryExprAST::Accept(std::shared_ptr<AstVisitor> visitor) const {
    return visitor->VisitBinaryExprAST(this);
}

Value * PrototypeAST::Accept(std::shared_ptr<AstVisitor> visitor) const  {
    return visitor->VisitPrototypeAST(this);
}

Value * FunctionAST::Accept(std::shared_ptr<AstVisitor> visitor) const   {
    return visitor->VisitFunctionAST(this);
}
