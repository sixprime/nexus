#pragma once

#include <string>
#include <sstream>
#include <toml++/toml.h>

namespace nexus::commands {

class AddCommand {
public:
    static int execute(const std::string& node_name, const std::string& git_url);

private:
    static bool update_toml_file(const std::string& node_name, const std::string& git_url);
    static void format_toml_value(std::ostringstream& oss, const toml::node& value);
};

} // namespace nexus::commands
