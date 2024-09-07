#include "aopch.h"
#include "settings.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

json load_settings()
{
    if (!std::filesystem::exists(".ao\\settings.json"))
        return R"({
    "startlist": [""],
    "suggestions": ["${dirs}"],
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
)"_json;

    std::ifstream f(".ao\\settings.json");
    return json::parse(f);
}
