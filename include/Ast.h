#include "AstBase.h"
#include <string>
#include <memory>
#include <vector>

#ifndef SAMMINE_LANG_AST_H
#define SAMMINE_LANG_AST_H
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
        public:
            std::vector<std::unique_ptr<DefinitionAST>> DefinitionVec;
        };

        class DefinitionAST : AstBase {};

        //! \brief A variable definition: "var x = expression;"
        class VarDefAST : public DefinitionAST {
        public:
            std::unique_ptr<TypedVarAST> TypedVar;
            std::unique_ptr<ExprAST> Expression;

            VarDefAST (std::unique_ptr<TypedVarAST> TypedVar, std::unique_ptr<ExprAST> Expression) :
                                            TypedVar(std::move(TypedVar)),
                                            Expression(std::move(Expression)) {};
        };

        //! \brief A prototype to present "func func_name(...) -> type;"

        //!
        //!
        class PrototypeAST : AstBase {};

        //! \brief A Function Definition that has the prototype and definition in terms of a block
        class FuncDefAST : public DefinitionAST {
        public:
            std::unique_ptr<PrototypeAST> Prototype;
            std::unique_ptr<BlockAST> Block;
        };

        //! \brief An AST to simulate a { } code block

        //!
        //!
        class BlockAST : AstBase {
            std::vector<std::unique_ptr<StmtAST>> Statements;
        };

        class StmtAST : AstBase {
        };

        class SimpleStmtAST : StmtAST {

        };

        class IfStmtAST : StmtAST {

        };

        class ExprAST : public AstBase {

        };

        class NumberExprAST : public ExprAST {
        public:
            std::string number;
        };

        class TypedVarAST : public AstBase {
        public:
            std::string name;
            std::string type;
        };

    }
}

#endif //SAMMINE_LANG_AST_H
