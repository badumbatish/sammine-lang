#include "Utilities.h"

namespace test_util {
    auto get_string_from_file(std::string file_name) -> std::string {
        auto file = FileRAII(file_name);
        assert(file.isOpen());
        auto input = file.getInternalStr();

        return input;
    }
}
