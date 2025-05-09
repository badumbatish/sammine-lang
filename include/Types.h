#pragma once
#include <map>
#include <memory>
#include <span>
#include <unordered_map>
#include <vector>
enum class TypeKind { I64_t, F64_t, Unit, Bool, Function, NonExistent, Error };

struct Type;
class FunctionType;

using TypePtr = std::shared_ptr<Type>;
class FunctionType {
  const std::vector<Type> total_types;

public:
  bool operator==(const FunctionType &t) const;

  bool operator<(const FunctionType &t) const;

  std::span<const Type> get_params_types() const;
  Type get_return_type() const;
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
  static Type Error() { return Type{TypeKind::Error, {}}; }
  static Type Unit() { return Type{TypeKind::Unit, {}}; }
  static Type NonExistent() { return Type{TypeKind::NonExistent, {}}; }
  static Type Function(std::vector<Type> params);

  Type(const Type &type)
      : type_kind(type.type_kind), type_data(type.type_data) {}
  Type(TypeKind type_kind, TypeData type_data)
      : type_kind(type_kind), type_data(type_data) {}

  Type operator=(const Type &other) const {
    return Type{other.type_kind, other.type_data};
  }
  bool operator==(const Type &other) const;

  bool operator!=(const Type &other) const;
  bool operator<(const Type &t) const;
  bool operator>(const Type &t) const;
};

struct TypeMapOrdering {
  std::map<Type, Type> type_map;
  std::vector<Type> visit_ancestor(const Type &t);
  std::optional<Type> lowest_common_type(const Type &a, const Type &b);
};
