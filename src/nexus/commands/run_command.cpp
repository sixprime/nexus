#include "nexus/commands/run_command.h"
#include "nexus/commands/build_command.h"
#include "nexus/utils/config_parser.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

namespace nexus::commands {

int RunCommand::execute(const std::vector<std::string>& args) {
    // First, ensure the project is built
    int build_result = BuildCommand::execute();
    if (build_result != 0) {
        std::cerr << "Build failed. Cannot run the project.\n";
        return build_result;
    }

    std::string executable_path = get_executable_path();
    if (executable_path.empty()) {
        std::cerr << "Could not find the executable.\n";
        return 1;
    }

    return run_executable(executable_path, args);
}

std::string RunCommand::get_executable_path() {
    auto node_opt = nexus::utils::ConfigParser::parse_toml("nexus.toml");
    if (!node_opt) {
        std::cerr << "Failed to parse nexus.toml\n";
        return "";
    }

    std::string executable_name = node_opt->name;
#ifdef _WIN32
    executable_name += ".exe";
#endif

    fs::path executable_path = fs::current_path() / "bin" / executable_name;
    
    if (!fs::exists(executable_path)) {
        std::cerr << "Executable not found at: " << executable_path << "\n";
        return "";
    }

    return executable_path.string();
}

int RunCommand::run_executable(const std::string& path, const std::vector<std::string>& args) {
    std::string command = "\"" + path + "\"";
    for (const auto& arg : args) {
        command += " " + arg;
    }

    std::cout << "Running: " << command << "\n";
    return std::system(command.c_str());
}

} // namespace nexus::commands
