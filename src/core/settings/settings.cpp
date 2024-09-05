#include "aopch.h"
#include "settings.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

json load_settings()
{
    if (!std::filesystem::exists(".ao\\settings.json"))
        return R"({
    "default_else_shell": true,
    "startlist": [""],
    "suggestions": ["${dirs}"],
    "commands": [
        {
            "do_index": true,
            "help": "",
            "tasks": [
                {
                    "names": [""],
                    "path": ""
                }
            ],
            "usage": [""]
        }
    ]
}
)"_json;

    std::ifstream f(".ao\\settings.json");
    return json::parse(f);
}
