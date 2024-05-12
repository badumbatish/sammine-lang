#ifndef SAMMINE_LANG_UTILITIES_H
#define SAMMINE_LANG_UTILITIES_H
#include <string>
#include <cassert>
#include "FileRAII.h"

namespace test_util {
    auto get_string_from_file(std::string file_name) -> std::string;

}


#endif