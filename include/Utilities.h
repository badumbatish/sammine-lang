#ifndef SAMMINE_LANG_UTILITIES_H
#define SAMMINE_LANG_UTILITIES_H
#include "FileRAII.h"
#include <cassert>
#include <string>

namespace sammine_util {
auto get_string_from_file(std::string file_name) -> std::string;

auto abort(const std::string &message = "<NO MESSAGE>") -> void;
} // namespace sammine_util

#endif
