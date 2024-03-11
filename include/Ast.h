#include "AstBase.h"
#include <string>
#include <memory>
#include <vector>

namespace sammine_lang {
    namespace AST {

        using Identifier = std::string;
        class ProgramAST;


        class DefinitionAST;
        class VarDefAST;
        class FuncDefAST;

        class PrototypeAST;
        class TypedVarAST;
        
        class ExprAST;
        class CallExprAST;
        class BinaryExprAST;
        class NumberExprAST;
        class VariableExprAST;

        class BlockAST;
        class ProgramAST : AstBase {
            std::vector<std::shared_ptr<DefinitionAST>> DefinitionVec;
        };

        class DefinitionAST : AstBase {};
        class VarDefAST : DefinitionAST {
            std::shared_ptr<TypedVarAST> TypedVar;
            double Val;
        };

        class PrototypeAST : AstBase {};
        class FuncDefAST : DefinitionAST {
            std::shared_ptr<PrototypeAST> Prototype;
            std::shared_ptr<BlockAST> Block;
        };
    }
}