#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
enum class TypeKind {
  I64_t,
  F64_t,
  Unit,
  Bool,
  Function,
};

struct Type;
struct FunctionType;

using TypePtr = std::shared_ptr<Type>;
struct FunctionType {
  std::vector<Type> total_types;

  bool operator==(const FunctionType &t) const;

  bool operator<(const FunctionType &t) const;

  FunctionType(const std::vector<Type> &total_types);
};
using TypeData = std::variant<std::monostate, FunctionType>;

struct Type {
  TypeKind type_kind;
  TypeData type_data;
  // Constructors
  static Type I64_t() { return Type{TypeKind::I64_t, {}}; }
  static Type F64_t() { return Type{TypeKind::F64_t, {}}; }
  static Type Bool() { return Type{TypeKind::Bool, {}}; }
  static Type Unit() { return Type{TypeKind::Unit, {}}; }
  static Type Function(std::vector<Type> params);

  bool operator==(const Type &other) const;

  bool operator!=(const Type &other) const;
  bool operator<(const Type &t) const;
  bool operator>(const Type &t) const;
};
