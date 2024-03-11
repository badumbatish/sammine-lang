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

        class StmtAST;
        class ProgramAST : AstBase {
            std::vector<std::shared_ptr<DefinitionAST>> DefinitionVec;
        };

        class DefinitionAST : AstBase {};

        //! \brief A variable definition: "var x = expression;"
        class VarDefAST : DefinitionAST {
            std::shared_ptr<TypedVarAST> TypedVar;
            std::shared_ptr<ExprAST> Expression;
        };

        //! \brief A prototype to present "func func_name(...) -> type;"

        //!
        //!
        class PrototypeAST : AstBase {};

        //! \brief A Function Definition that has the prototype and definition in terms of a block
        class FuncDefAST : DefinitionAST {
            std::shared_ptr<PrototypeAST> Prototype;
            std::shared_ptr<BlockAST> Block;
        };

        //! \brief An AST to simulate a { } code block

        //!
        //!
        class BlockAST : AstBase {
            std::vector<StmtAST> Statements;
        };

    }
}