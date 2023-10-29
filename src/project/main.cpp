#include <iostream>
#include "lexer.h"
#include "parser.h"
int main() {

    // Prime the first token.
    fprintf(stderr, "ready> ");
    parser::getNextToken();

    // Make the module, which holds all the code.
    parser::InitializeModule();

    // Run the main "interpreter loop" now.

    parser::MainLoop();

    // Print out all of the generated code.
    TheModule->print(llvm::errs(), nullptr);

    return 0;
}