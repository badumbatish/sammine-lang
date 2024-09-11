
#include "ScopeGeneratorVisitor.h"
#include "Utilities.h"
#include "tl/expected.hpp"
#include <catch2/catch_test_macros.hpp>

using namespace sammine_lang;
TEST_CASE("Empty program scope", "[Scoping]") {

  AST::ScopeGeneratorVisitor scopegen_visitor;
}
