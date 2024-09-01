#include "aopch.h"
#include "entrypoint.h"
#include "ao.h"

#include "argparse/argparse.h"
#include "fileio/foldersystem.h"
#include "fileio/filesystem.h"
#include "console/console.h"

#include "core/readf/readf.h"
#include "core/execute/execute.h"

int take_entry(const std::vector<std::string> args)
{
    argparse parser = argparse("AO", "A developer tool made by a developer for developers", unrecognized_argument_error);
    parser.add({"-h", "--help", "/?", "-?"}, "Show help message", "", true, false);
    parser.add({"-i", "--init"}, "Initialize in current directory", "", true, false);
    parser.add({"-a", "--api"}, "Run in server mode and accept API requests", "", true, false);

    std::vector<argparse::parsed_argument> parsed_args = parser.parse(args);

    if (parsed_args.size() > 0)
        return exec_parsed_args(parser, parsed_args);

    else
    {
        setup(); // show a setup screen with some basic details on first boot
        AO::clear_console();

        int is_running = 1; // 1 = true; 0 = false
        while (is_running)
        {
            AO::print_prompt();
            console::readf readf = console::readf({""});
            is_running = execute(readf.takeinput());
        }
    }

    return 0;
}

void unrecognized_argument_error(const std::string& err)
{
    console::throw_error(err, "Unrecognized argument");
}

// initialize .ao folder with it's subdirectories
// this folder will contain all the settings for AO
void init_folders()
{
    foldersystem::create(".ao");
    foldersystem::create(".ao\\etc");
    filesystem::write(".ao\\settings.json", "{}\n");
}
