#include "nexus/commands/add_command.h"
#include "nexus/commands/remove_command.h"
#include "nexus/commands/fetch_command.h"
#include "nexus/commands/build_command.h"
#include "nexus/commands/run_command.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty()) {
        std::cerr << "Usage: nexus <command> [arguments]\n";
        return 1;
    }

    std::string command = args[0];

    if (command == "add") {
        if (args.size() < 3) {
            std::cerr << "Usage: nexus add <node_name> --git=<git_url>\n";
            return 1;
        }
        std::string node_name = args[1];
        std::string git_url = args[2].substr(6); // Remove "--git=" prefix
        return nexus::commands::AddCommand::execute(node_name, git_url);
    } else if (command == "remove") {
        if (args.size() < 2) {
            std::cerr << "Usage: nexus remove <node_name>\n";
            return 1;
        }
        std::string node_name = args[1];
        return nexus::commands::RemoveCommand::execute(node_name);
    } else if (command == "fetch") {
        return nexus::commands::FetchCommand::execute();
    } else if (command == "build") {
        return nexus::commands::BuildCommand::execute();
    } else if (command == "run") {
        return nexus::commands::RunCommand::execute(std::vector<std::string>(args.begin() + 1, args.end()));
    } else {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }
}
