#include "AstBase.h"
#include <string>

namespace sammine_lang {
    namespace AST {

        using Identifier = std::string;
        class ProgramAST;
        class DefinitionAST;
        class VarDefAST;
        class FuncDefAST;

        class TypedVarAST;

        class ExprAST;
        class CallExprAST;
        class BinaryExprAST;
        class NumberExprAST;
        class VariableExprAST;
        
        class ProgramAST : AstBase {

        };
    }
}