#pragma once
// A simple scoping class, doesn't differentiate between different names, like
// variable name, func name and all that
#include <set>
template <class T> class LexicalContext {
  std::shared_ptr<LexicalContext> parent_scope;
  std::set<std::string> symbols;
  std::unordered_map<std::string, T> symbols_to_loc;

public:
  explicit LexicalContext() {}
  LexicalContext(std::shared_ptr<LexicalContext> parent_scope)
      : parent_scope(parent_scope) {}

  enum NameQueryResult {
    nameFound,
    nameNotFound,
  };
  void registerNameLocation(const std::string &name, T l) {
    symbols.insert(name);
    symbols_to_loc[name] = l;
  }
  NameQueryResult queryName(const std::string &name) const {
    return symbols.contains(name) ? nameFound : nameNotFound;
  }

  T get_location(const std::string &name) { return symbols_to_loc.at(name); }

  NameQueryResult recursiveQueryName(const std::string &name) const {
    if (symbols.find(name) != symbols.end())
      return nameFound;
    else if (parent_scope == nullptr)
      return nameNotFound;
    return parent_scope->recursiveQueryName(name);
  }
}; // namespace sammine_lang::AST
