#include "aopch.h"
#include "settings.h"
#include "ao.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::string settings_format = R"({
    "startlist": [""],
    "suggestions": [
        "exit",
        "_cws", "--workspace",
        "help", "--help", "-h", "-?", "/?",
        "cls", "_clear", "_cls",
        "_initAO", "-i", "--init",
        "-aod", "--diagxt",
        "--setup", "${dirs}"
    ],
    "commands": [
        {
            "names": [""],
            "paths": [""],
            "help": "",
            "usage": [""],
            "do_index": true
        }
    ]
}
)";

json load_settings()
{
    const std::string env_path = AO::ao_env_path + "\\.ao\\settings.json";
    if (!std::filesystem::exists(env_path))
        return json::parse(settings_format);

    std::ifstream f(env_path);
    return json::parse(f);
}
