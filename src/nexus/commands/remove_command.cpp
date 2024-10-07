#include "nexus/commands/remove_command.h"
#include "nexus/utils/config_parser.h"
#include <iostream>
#include <fstream>
#include <toml++/toml.h>

namespace nexus::commands {

int RemoveCommand::execute(const std::string& node_name) {
    if (update_toml_file(node_name)) {
        std::cout << "Successfully removed " << node_name << " from nexus.toml\n";
        return 0;
    } else {
        std::cerr << "Failed to remove " << node_name << " from nexus.toml\n";
        return 1;
    }
}

bool RemoveCommand::update_toml_file(const std::string& node_name) {
    try {
        auto config = toml::parse_file("nexus.toml");
        
        if (config.contains("links")) {
            auto* links = config["links"].as_table();
            if (!links) {
                std::cerr << "Error: 'links' is not a table in nexus.toml\n";
                return false;
            }
            
            links->erase(node_name);
        }

        std::ofstream file("nexus.toml");
        file << config;

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error updating nexus.toml: " << e.what() << "\n";
        return false;
    }
}

} // namespace nexus::commands