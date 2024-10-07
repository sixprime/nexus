#pragma once

#include <string>
#include <vector>
#include "nexus/core/node.h"

namespace nexus::commands {

class BuildCommand {
public:
    static int execute();

private:
    static bool generate_ninja_file(const nexus::core::Node& node);
    static std::vector<std::string> get_source_files();
    static std::string get_cpp_standard_flag(nexus::core::CppStandard standard);
};

} // namespace nexus::commands
