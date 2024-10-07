#include "nexus/utils/config_parser.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <toml++/toml.h>

namespace nexus::utils {

std::optional<nexus::core::Node> ConfigParser::parse_toml(const std::string& file_path) {
    try {
        std::cout << "Attempting to parse file: " << file_path << std::endl;
        auto config = toml::parse_file(file_path);
        
        nexus::core::Node node;

        // Parse [node] section
        std::cout << "Parsing [node] section..." << std::endl;
        if (auto node_table = config["node"].as_table()) {
            node.name = node_table->get("name") ? node_table->get("name")->as_string()->get() : "";
            node.version = node_table->get("version") ? parse_version(node_table->get("version")->as_string()->get()) : nexus::core::Version{0, 1, 0};
            node.description = node_table->get("description") ? node_table->get("description")->as_string()->get() : "";
            node.license = node_table->get("license") ? node_table->get("license")->as_string()->get() : "";
            node.readme = node_table->get("readme") ? node_table->get("readme")->as_string()->get() : "";

            if (auto authors = node_table->get("authors")) {
                if (auto authors_array = authors->as_array()) {
                    for (const auto& author : *authors_array) {
                        node.authors.push_back(author.as_string()->get());
                    }
                }
            }
        } else {
            std::cerr << "Warning: [node] section not found in TOML file." << std::endl;
        }

        // Parse [build] section
        std::cout << "Parsing [build] section..." << std::endl;
        if (auto build_table = config["build"].as_table()) {
            node.build_config.type = build_table->get("type") ? 
                string_to_build_type(build_table->get("type")->as_string()->get()) : 
                nexus::core::BuildType::Executable;
            node.build_config.standard = build_table->get("standard") ? 
                string_to_cpp_standard(build_table->get("standard")->as_string()->get()) : 
                nexus::core::CppStandard::Cpp17;
        } else {
            std::cerr << "Warning: [build] section not found in TOML file." << std::endl;
        }

        // Parse [links] section
        std::cout << "Parsing [links] section..." << std::endl;
        if (auto links_table = config["links"].as_table()) {
            for (const auto& [key, value] : *links_table) {
                nexus::core::Link link;
                if (auto git = value.as_table()->get("git")) {
                    link.git_url = git->as_string()->get();
                }
                if (auto version = value.as_table()->get("version")) {
                    link.version = parse_version(version->as_string()->get());
                }
                node.links[std::string(key)] = link;
            }
        } else {
            std::cerr << "Warning: [links] section not found in TOML file." << std::endl;
        }

        // Parse [toolchain] section
        std::cout << "Parsing [toolchain] section..." << std::endl;
        if (auto toolchain_table = config["toolchain"].as_table()) {
            node.toolchain.compiler = toolchain_table->get("compiler") ? 
                toolchain_table->get("compiler")->as_string()->get() : "";
            node.toolchain.target = toolchain_table->get("target") ? 
                toolchain_table->get("target")->as_string()->get() : "";
        } else {
            std::cerr << "Warning: [toolchain] section not found in TOML file." << std::endl;
        }

        // Parse [profile] section
        std::cout << "Parsing [profile] section..." << std::endl;
        if (auto profiles_table = config["profile"].as_table()) {
            for (const auto& [key, value] : *profiles_table) {
                nexus::core::Profile profile;
                profile.name = std::string(key);
                if (auto profile_table = value.as_table()) {
                    if (auto opt_level = profile_table->get("opt-level")) {
                        profile.opt_level = opt_level->as_integer()->get();
                    }
                    if (auto lto = profile_table->get("lto")) {
                        profile.lto = lto->as_boolean()->get();
                    }
                    if (auto strip = profile_table->get("strip")) {
                        profile.strip = strip->as_boolean()->get();
                    }
                }
                node.profiles.push_back(profile);
            }
        } else {
            std::cerr << "Warning: [profile] section not found in TOML file." << std::endl;
        }

        std::cout << "TOML parsing completed successfully." << std::endl;
        return node;
    } catch (const toml::parse_error& err) {
        std::cerr << "TOML parsing failed: " << err.description() << std::endl
                  << "Error occurred at line " << err.source().begin.line 
                  << ", column " << err.source().begin.column << std::endl;
        return std::nullopt;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing nexus.toml: " << e.what() << std::endl;
        return std::nullopt;
    }
}

nexus::core::BuildType ConfigParser::string_to_build_type(const std::string& str) {
    if (str == "library") return nexus::core::BuildType::Library;
    return nexus::core::BuildType::Executable;
}

nexus::core::CppStandard ConfigParser::string_to_cpp_standard(const std::string& str) {
    if (str == "c++11") return nexus::core::CppStandard::Cpp11;
    if (str == "c++14") return nexus::core::CppStandard::Cpp14;
    if (str == "c++20") return nexus::core::CppStandard::Cpp20;
    return nexus::core::CppStandard::Cpp17;
}

nexus::core::Version ConfigParser::parse_version(const std::string& version_str) {
    std::regex version_regex(R"((\d+)\.(\d+)\.(\d+))");
    std::smatch match;
    if (std::regex_match(version_str, match, version_regex)) {
        return {
            std::stoi(match[1].str()),
            std::stoi(match[2].str()),
            std::stoi(match[3].str())
        };
    }
    return {0, 0, 0};
}

} // namespace nexus::utils
