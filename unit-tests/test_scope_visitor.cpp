
#include "AST/ScopeGeneratorVisitor.h"
#include <catch2/catch_test_macros.hpp>

using namespace sammine_lang;
TEST_CASE("Empty program scope", "[Scoping]") {

  AST::ScopeGeneratorVisitor scopegen_visitor;
}
