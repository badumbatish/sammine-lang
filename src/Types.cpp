#include "Types.h"
bool FunctionType::operator<<(const FunctionType &t) const {
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
