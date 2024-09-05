#include "aopch.h"
#include "execute.h"
#include "ao.h"

#include "core/entrypoint/entrypoint.h"
#include "console/console.h"
#include "strings/strings.h"

void AOs1000()
{
    console::print("AOs1000!", console::color::LIGHT_WHITE);
    console::print("CONGRATULATIONS! For hitting 1000 LINES OF CODE in AOs 1.3!", console::color::LIGHT_WHITE);
    console::print("It was my first ever program to reach these many LINES OF CODE!", console::color::LIGHT_WHITE);
}

void BadApple()
{
    system("start https://youtu.be/2Ni13dnAbSA");
}

void TommyViceCity()
{
    system("cls");
    console::print("Lucia... ", console::color::LIGHT_MAGENTA, false);
    console::print("  (Do you know why you're here?)  ", console::color::LIGHT_WHITE, false);
    console::print("Bad luck, I guess.", console::color::GRAY);
}

void Rockstar6()
{
    system("start https://youtu.be/QdBZY2fkU-0");
}

void itanimulli()
{
    console::print("spelling mistake kid. It's illuminati", console::color::LIGHT_WHITE);
}

void illuminati()
{
    system("start https://youtu.be/DCqty4-VjZc");
}

void wgpcgr()
{
    console::print("true", console::get_console_back_color(), true);
}

void mrstark()
{
    console::print("And IAmBatMan.", console::get_console_back_color(), true);
}

void itsmagic()
{
    system("start https://youtu.be/dQw4w9WgXcQ");
}

void diagxt()
{
    std::cout << "diagxt\n";
}

void help()
{
    std::cout << "help\n";
}

std::map<std::vector<std::string>, std::function<void()>> cmd_func_map = {
    {{"_AOs1000"}, AOs1000},
    {{"--setup"}, run_setup},
    {{"cls", "_clear", "_cls"}, AO::clear_console},
    {{"_BadApple"}, BadApple},
    {{"_tommyViceCity"}, TommyViceCity},
    {{"_R*6"}, Rockstar6},
    {{"_itanimulli"}, itanimulli},
    {{"_illuminati"}, illuminati},
    {{"_withgreatpowercomesgreatresponsibility", "-wgpcgr"}, wgpcgr},
    {{"_IAmIronMan", "-mrstark"}, mrstark},
    {{"_itsmagicitsmagic"}, itsmagic},
    {{"--diagxt", "-aod"}, diagxt},
    {{"help", "--help", "-h", "-?", "/?"}, help},
    {{"_iws", "--init", "-iws"}, init_folders}
};

std::function<void()> get_command_func(const std::string& cmd)
{
    for (const auto& [key, pair] : cmd_func_map)
    {
        if (strings::any(cmd, key, true))
            return pair;
    }

    return nullptr;
}
