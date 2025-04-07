#include "Types.h"
#include <string_view>
#include <unordered_map>
bool FunctionType::operator<(const FunctionType &t) const {
  if (total_types.size() != t.total_types.size())
    return false;

  for (size_t i = 0; i < total_types.size(); i++) {
    if (!(total_types[i] < t.total_types[i]))
      return false;
  }

  return true;
}
bool FunctionType::operator==(const FunctionType &t) const {
  return total_types == t.total_types;
}
FunctionType::FunctionType(const std::vector<Type> &total_types) {
  this->total_types = total_types;
}

Type Type::Function(std::vector<Type> params) {
  return Type{TypeKind::Function, FunctionType{params}};
}
bool Type::operator!=(const Type &other) const { return !(operator==(other)); }
bool Type::operator<(const Type &t) const { return this->operator==(t); }
bool Type::operator>(const Type &t) const { return this->operator==(t); }
bool Type::operator==(const Type &other) const {
  if (this->type_kind != other.type_kind)
    return false;
  if (this->type_kind != TypeKind::Function)
    return true;

  auto ftype = std::get<FunctionType>(this->type_data);
  FunctionType other_ftype = std::get<FunctionType>(other.type_data);

  return ftype == other_ftype;
}
