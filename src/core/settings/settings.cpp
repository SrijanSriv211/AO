#include "aopch.h"
#include "settings.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::string settings_format = R"({
    "startlist": [""],
    "suggestions": ["help", "--help", "-h", "-?", "/?", "cls", "exit", "_clear", "_cls", "_initAO", "-i", "-aod", "--init", "--diagxt", "--setup", "${dirs}"],
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
    if (!std::filesystem::exists(".ao\\settings.json"))
        return json::parse(settings_format);

    std::ifstream f(".ao\\settings.json");
    return json::parse(f);
}
