#include "aopch.h"
#include "entrypoint.h"

#include "argparse/argparse.h"
#include "fileio/foldersystem.h"
#include "console/console.h"

int take_entry(const std::vector<std::string> args)
{
    argparse parser = argparse("AO", "A developer tool made by a developer for developers", unrecognized_argument_error);
    parser.add({"-h", "--help", "/?", "-?"}, "Show help message", "", true, false);

    std::vector<argparse::parsed_argument> parsed_args = parser.parse(args);

    if (parsed_args.size() > 0)
        return exec_parsed_args(parser, parsed_args);

    else
    {
        setup(); // show a setup screen with some basic details on first boot
        exec_code(NULL);
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
    foldersystem::create(".ao/root");
    foldersystem::create(".ao/etc");
}
