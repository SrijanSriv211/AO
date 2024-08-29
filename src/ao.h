#pragma once

namespace AO
{
    extern const std::string about_AO;
    extern const std::string AO_repo_link;
    extern const std::filesystem::path ao_env_path;
    std::string get_root_path();
    void clear_console();
    void print_prompt();
}
