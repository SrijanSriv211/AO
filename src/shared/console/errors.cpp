#include "aopch.h"
#include "console.h"

namespace console
{
    namespace errors
    {
        std::string runtime(const std::string& command, const std::string& message)
        {
            console::print(command + ": ", console::color::LIGHT_WHITE, false);
            console::print("runtime error", console::color::BLACK, console::color::LIGHT_RED, false);
            console::print(": " + message, console::color::WHITE);

            return command + ": runtime error: " + message;
        }

        std::string syntax(const std::string& message)
        {
            console::print("syntax error", console::color::BLACK, console::color::LIGHT_RED, false);
            console::print(": " + message, console::color::WHITE);

            return "syntax error: " + message;
        }
    }
}
