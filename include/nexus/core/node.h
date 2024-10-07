#pragma once

#include <optional>
#include <string>
#include <vector>
#include <map>

namespace nexus::core {

enum class BuildType {
    Executable,
    Library
};

enum class CppStandard {
    Cpp11,
    Cpp14,
    Cpp17,
    Cpp20
};

struct Version {
    int major = 0;
    int minor = 1;
    int patch = 0;
};

struct BuildConfig {
    BuildType type = BuildType::Executable;
    CppStandard standard = CppStandard::Cpp20;
};

struct Link {
    std::optional<Version> version;
    std::optional<std::string> git_url;
    std::optional<std::string> alias;
};

struct Toolchain {
    std::string compiler;
    std::string target;
};

struct Profile {
    std::string name;
    int opt_level = 0;
    std::optional<bool> strip;
    std::optional<bool> lto;
};

struct Node {
    std::string name;
    Version version;
    std::vector<std::string> authors;
    std::string description;
    std::string license;
    std::string readme;
    BuildConfig build_config;
    std::map<std::string, Link> links;
    Toolchain toolchain;
    std::vector<Profile> profiles;
};

} // namespace nexus::core
