#include "Utilities.h"
#include <cstdlib>

namespace test_util {
    auto get_string_from_file(std::string file_name) -> std::string {
        auto file = FileRAII(file_name);
        assert(file.isOpen());
        auto input = file.getInternalStr();

        return input;
    }

    auto abort(const std::string &message) -> void {
        std::cerr << "\033[1;31m" << "ERROR" << "\033[0m : " << message << std::endl;
        std::abort();
    }

}
