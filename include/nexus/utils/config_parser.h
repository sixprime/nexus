#pragma once

#include "nexus/core/node.h"
#include <optional>
#include <string>

namespace nexus::utils {

class ConfigParser {
public:
    static std::optional<nexus::core::Node> parse_toml(const std::string& file_path);

private:
    static nexus::core::BuildType string_to_build_type(const std::string& str);
    static nexus::core::CppStandard string_to_cpp_standard(const std::string& str);
    static nexus::core::Version parse_version(const std::string& version_str);
};

} // namespace nexus::utils
