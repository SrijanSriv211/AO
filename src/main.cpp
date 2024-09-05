#include "aopch.h"
#include "core/entrypoint/entrypoint.h"

int main(int argc, char const *argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    args.erase(args.begin());
    int return_code = 0;

    while (true)
    {
        if (take_entry(args) == 0)
            break;
    }

    return return_code;
}
