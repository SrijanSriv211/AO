#include "aopch.h"
#include "entrypoint.h"

#include "fileio/filesystem.h"
#include "console/console.h"
#include "strings/strings.h"

#include "ao.h"

void pause(const std::string& msg)
{
    std::cout << msg;
    std::cin.get();
}

void setup()
{
    std::string initbootfile = AO::get_root_path() + "\\initboot";
    if (std::filesystem::exists(initbootfile))
        return;

    filesystem::write(initbootfile, "1");
    run_setup();
}

void run_setup()
{
    system("cls");

    // https://onlinetools.com/ascii/convert-text-to-ascii-art#tool
    std::vector<std::string> HelloText = {
        " _    _      _ _       _ ",
        "| |  | |    | | |     | |",
        "| |__| | ___| | | ___ | |",
        "|  __  |/ _ \\ | |/ _ \\| |",
        "| |  | |  __/ | | (_) |_|",
        "|_|  |_|\\___|_|_|\\___/(_)",
        "                          "
    };

    std::vector<std::string> WelcomeToAO = {
        " __      __   _                    _           _   ___       ",
        " \\ \\    / /__| |__ ___ _ __  ___  | |_ ___    /_\\ / _ \\ ___  ",
        "  \\ \\/\\/ / -_) / _/ _ \\ '  \\/ -_) |  _/ _ \\  / _ \\ (_) (_-<_ ",
        "   \\_/\\_/\\___|_\\__\\___/_|_|_\\___|  \\__\\___/ /_/ \\_\\___//__(_)",
        "                                                           "
    };

    console::print(strings::join("\n", HelloText), console::color::LIGHT_WHITE);
    console::print(strings::join("\n", WelcomeToAO), console::color::LIGHT_YELLOW);

    pause("Press any key to continue.");

    console::print("The ", console::color::GRAY, false);
    console::print("help ", console::color::LIGHT_WHITE, false);
    console::print("command will give you information about any command that is indexed by AO.", console::color::GRAY);
    console::print("So, to know which commands are indexed and which are not and to change that setting you can open ", console::color::GRAY, false);
    console::print("`.ao/root/settings.json`.", console::color::LIGHT_WHITE);
    console::print("And furthermore you can visit ", console::color::GRAY, false);
    console::print("`https://github.com/SrijanSriv211/AO` ", console::color::LIGHT_WHITE, false);
    console::print("to get more information about AO ", console::color::GRAY, false);
    console::print("`settings.json`.", console::color::LIGHT_WHITE);

    pause("\nNow you know the basics of how to use AO.\nPress any key to continue.");

    std::vector<std::string> ThankYou = {
        "  _____ _              _    __   __          ",
        " |_   _| |_  __ _ _ _ | |__ \\ \\ / /__ _  _   ",
        "   | | | ' \\/ _` | ' \\| / /  \\ V / _ \\ || |_ ",
        "   |_| |_||_\\__,_|_||_|_\\_\\   |_|\\___/\\_,_(_)",
        "                                             "
    };

    console::print(strings::join("\n", ThankYou), console::color::LIGHT_GREEN);
    pause("");
}
