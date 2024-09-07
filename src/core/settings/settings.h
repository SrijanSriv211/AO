#pragma once

#include "nlohmann/json.hpp"
using json = nlohmann::json;

extern std::string settings_format;
json load_settings();
