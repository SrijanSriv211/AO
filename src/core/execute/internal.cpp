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
    std:: cout << std::endl;
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
    std::cout << std::endl;
}

void illuminati()
{
    system("start https://youtu.be/DCqty4-VjZc");
}

void wgpcgr()
{
    console::print("true", console::get_console_back_color(), true);
    std::cout << std::endl;
}

void mrstark()
{
    console::print("And IAmBatMan.", console::get_console_back_color(), true);
    std::cout << std::endl;
}

void itsmagic()
{
    system("start https://youtu.be/dQw4w9WgXcQ");
}

void diagxt()
{
    std::cout << "diagxt\n";
    std::cout << std::endl;
}

void help()
{
    std::cout << "HELP\n";
    std::cout << std::endl;
}

std::map<std::vector<std::string>, std::function<void()>> cmd_func_map = {
    {{"--AOs1000"}, AOs1000},
    {{"--setup"}, run_setup},
    {{"--clear", "-cls"}, AO::clear_console},
    {{"--BadApple"}, BadApple},
    {{"--tommyViceCity"}, TommyViceCity},
    {{"-R*6"}, Rockstar6},
    {{"--itanimulli"}, itanimulli},
    {{"--illuminati"}, illuminati},
    {{"--withgreatpowercomesgreatresponsibility", "-wgpcgr"}, wgpcgr},
    {{"--IAmIronMan", "-mrstark"}, mrstark},
    {{"--itsmagicitsmagic"}, itsmagic},
    {{"--diagxt", "-aod"}, diagxt},
    {{"--help", "-h", "-?", "/?"}, help}
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