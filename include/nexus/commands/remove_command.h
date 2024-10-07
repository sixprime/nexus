#pragma once

#include <string>

namespace nexus::commands {

class RemoveCommand {
public:
    static int execute(const std::string& node_name);

private:
    static bool update_toml_file(const std::string& node_name);
};

} // namespace nexus::commands
