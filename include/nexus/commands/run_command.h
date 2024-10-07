#pragma once

#include <string>
#include <vector>

namespace nexus::commands {

class RunCommand {
public:
    static int execute(const std::vector<std::string>& args);

private:
    static std::string get_executable_path();
    static int run_executable(const std::string& path, const std::vector<std::string>& args);
};

} // namespace nexus::commands
