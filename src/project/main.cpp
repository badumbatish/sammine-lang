#include <iostream>
#include "lexer.h"
#include "parser.h"
int main() {
    fprintf(stderr, "ready> ");
    parser::getNextToken();

    // Run the main "interpreter loop" now.
    parser::MainLoop();
}