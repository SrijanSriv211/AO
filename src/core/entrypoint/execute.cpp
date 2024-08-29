#include "aopch.h"
#include "entrypoint.h"
#include "ao.h"

#include "core/readf/readf.h"
#include "core/lexer/lex.h"

void exec_code(const std::string* code)
{
    // if code is null execute it in AO shell (much like python's shell or CMD)
    if (code == nullptr)
    {
        AO::clear_console();
        AO::print_prompt();

        console::readf readf = console::readf({""});
        std::vector<lex::token> tokens = readf.takeinput();

        // break tokens into `cmd` and `args` for the AO parser
        lex::token cmd = tokens.front();
        std::vector<lex::token> args = std::vector<lex::token>(tokens.begin() + 1, tokens.end());

        std::cout << cmd.type << " : [" << cmd.name << "]\n";
        for (std::vector<lex::token>::size_type i = 0; i < args.size(); i++)
            std::cout << args[i].type << " : [" << args[i].name << "]\n";
    }

    else
    {
        lex lexer(*code);

        // break tokens into `cmd` and `args` for the AO parser
        lex::token cmd = lexer.tokens.front();
        std::vector<lex::token> args = std::vector<lex::token>(lexer.tokens.begin() + 1, lexer.tokens.end());

        std::cout << cmd.type << " : [" << cmd.name << "]\n";
        for (std::vector<lex::token>::size_type i = 0; i < args.size(); i++)
            std::cout << args[i].type << " : [" << args[i].name << "]\n";
    }
}
