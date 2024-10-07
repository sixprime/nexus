#include "nexus/commands/fetch_command.h"
#include "nexus/utils/config_parser.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

namespace nexus::commands {

int FetchCommand::execute() {
    auto node_opt = nexus::utils::ConfigParser::parse_toml("nexus.toml");
    if (!node_opt) {
        std::cerr << "Failed to parse nexus.toml\n";
        return 1;
    }

    if (!create_external_directory()) {
        std::cerr << "Failed to create external directory\n";
        return 1;
    }

    if (fetch_dependencies(node_opt->links)) {
        std::cout << "Successfully fetched all dependencies\n";
        return 0;
    } else {
        std::cerr << "Failed to fetch some dependencies\n";
        return 1;
    }
}

bool FetchCommand::fetch_dependencies(const std::map<std::string, nexus::core::Link>& links) {
    bool all_success = true;
    for (const auto& [name, link] : links) {
        if (!link.git_url) {
            std::cerr << "No git URL specified for " << name << "\n";
            all_success = false;
            continue;
        }

        std::cout << "Fetching " << name << "...\n";
        if (!add_git_submodule(name, *link.git_url)) {
            std::cerr << "Failed to fetch " << name << "\n";
            all_success = false;
        }
    }
    return all_success;
}

bool FetchCommand::add_git_submodule(const std::string& name, const std::string& url) {
    std::string command = "git submodule add " + url + " external/" + name;
    int result = std::system(command.c_str());
    return result == 0;
}

bool FetchCommand::create_external_directory() {
    try {
        fs::create_directory("external");
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating external directory: " << e.what() << "\n";
        return false;
    }
}

} // namespace nexus::commands
