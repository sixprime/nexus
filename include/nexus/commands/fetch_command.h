#pragma once

#include <string>
#include <map>
#include "nexus/core/node.h"

namespace nexus::commands {

class FetchCommand {
public:
    static int execute();

private:
    static bool fetch_dependencies(const std::map<std::string, nexus::core::Link>& links);
    static bool add_git_submodule(const std::string& name, const std::string& url);
    static bool create_external_directory();
};

} // namespace nexus::commands
