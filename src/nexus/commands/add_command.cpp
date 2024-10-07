#include "nexus/commands/add_command.h"
#include "nexus/utils/config_parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <toml++/toml.h>

namespace nexus::commands {

int AddCommand::execute(const std::string& node_name, const std::string& git_url) {
    if (update_toml_file(node_name, git_url)) {
        std::cout << "Successfully added " << node_name << " to nexus.toml\n";
        return 0;
    } else {
        std::cerr << "Failed to add " << node_name << " to nexus.toml\n";
        return 1;
    }
}

bool AddCommand::update_toml_file(const std::string& node_name, const std::string& git_url) {
    try {
        toml::table config;
        if (std::ifstream is("nexus.toml"); is) {
            config = toml::parse(is);
        }
        
        if (!config.contains("links")) {
            config.insert("links", toml::table{});
        }

        auto& links = *config["links"].as_table();
        links.insert_or_assign(node_name, toml::table{{"git", git_url}});

        // Manually format the TOML output
        std::ostringstream oss;
        for (const auto& [key, value] : config) {
            if (key == "links") {
                oss << "[links]\n";
                for (const auto& [link_name, link_value] : *value.as_table()) {
                    oss << link_name << " = { ";
                    if (const auto* git = link_value.as_table()->get("git")) {
                        oss << "git = \"" << git->as_string()->get() << "\"";
                    }
                    oss << " }\n";
                }
            } else {
                oss << "[" << key << "]\n";
                for (const auto& [subkey, subvalue] : *value.as_table()) {
                    oss << subkey << " = ";
                    format_toml_value(oss, subvalue);
                    oss << "\n";
                }
            }
            oss << "\n";
        }

        std::ofstream file("nexus.toml");
        file << oss.str();

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error updating nexus.toml: " << e.what() << "\n";
        return false;
    }
}

void AddCommand::format_toml_value(std::ostringstream& oss, const toml::node& value) {
    if (value.is_string()) {
        oss << "\"" << value.as_string()->get() << "\"";
    } else if (value.is_integer()) {
        oss << value.as_integer()->get();
    } else if (value.is_floating_point()) {
        oss << value.as_floating_point()->get();
    } else if (value.is_boolean()) {
        oss << (value.as_boolean()->get() ? "true" : "false");
    } else if (value.is_array()) {
        oss << "[";
        bool first = true;
        for (const auto& elem : *value.as_array()) {
            if (!first) oss << ", ";
            format_toml_value(oss, elem);
            first = false;
        }
        oss << "]";
    } else if (value.is_table()) {
        oss << "{ ";
        bool first = true;
        for (const auto& [k, v] : *value.as_table()) {
            if (!first) oss << ", ";
            oss << k << " = ";
            format_toml_value(oss, v);
            first = false;
        }
        oss << " }";
    } else {
        oss << "null";  // Or handle other types as needed
    }
}

} // namespace nexus::commands
