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
            std::vector<std::shared_ptr<DefinitionAST>> DefinitionVec;
        };

        class DefinitionAST : AstBase {};

        //! \brief A variable definition: "var x = expression;"
        class VarDefAST : public DefinitionAST {
        public:
            std::shared_ptr<TypedVarAST> TypedVar;
            std::shared_ptr<ExprAST> Expression;

            VarDefAST (std::shared_ptr<TypedVarAST> TypedVar, std::shared_ptr<ExprAST> Expression) :
                                            TypedVar(TypedVar),
                                            Expression(Expression) {};
        };

        //! \brief A prototype to present "func func_name(...) -> type;"

        //!
        //!
        class PrototypeAST : AstBase {};

        //! \brief A Function Definition that has the prototype and definition in terms of a block
        class FuncDefAST : public DefinitionAST {
        public:
            std::shared_ptr<PrototypeAST> Prototype;
            std::shared_ptr<BlockAST> Block;
        };

        //! \brief An AST to simulate a { } code block

        //!
        //!
        class BlockAST : AstBase {
            std::vector<std::shared_ptr<StmtAST>> Statements;
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
