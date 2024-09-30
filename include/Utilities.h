#ifndef SAMMINE_LANG_UTILITIES_H
#define SAMMINE_LANG_UTILITIES_H
#include "FileRAII.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
namespace sammine_util {

//! A class representing a location for sammine-lang, this is helpful in
//! debugging
//! .
//! .
class Location {
public:
  size_t line;
  size_t column;
  size_t source_start; // Refers to the true location in the orignal source
                       // code string
  size_t source_end;
  Location() : line(0), column(0), source_start(0), source_end(0) {}

  Location(const Location &loc_a, const Location &loc_b) {}
  inline void advance() {
    column++;
    source_end++;
  }
  inline void devance() {
    if (column == 0)
      return;

    column--;
    source_end--;
  }
  inline void newLine() {
    line++;
    source_end++;
    column = 0;
  }

  friend std::ostream &operator<<(std::ostream &out, const Location &loc) {
    return out << loc.line << ":" << loc.column;
  }
  std::string to_string() const {
    return std::to_string(this->line) + ":" + std::to_string(this->column);
  }

  bool operator==(const Location &other) {
    return source_start == other.source_start && source_end == other.source_end;
  }
  operator std::string() { return this->to_string(); }
};

auto get_string_from_file(std::string file_name) -> std::string;
inline size_t unique_ast_id = 0;
auto abort(const std::string &message = "<NO MESSAGE>") -> void;
auto abort_on(bool abort_if_true,
              const std::string &message = "<NO MESSAGE>") -> void;

class ErrorMsgs {
  using ErrorMsg = std::pair<Location, std::string>;

public:
  std::vector<ErrorMsg> errors;

  std::vector<std::string> stringify() {
    std::vector<std::string> string_errors;
    for (auto &error : errors) {
      string_errors.push_back(error.second);
    }

    return string_errors;
  }
};

} // namespace sammine_util

#endif
