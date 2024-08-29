#include "aopch.h"
#include "execute.h"

#include "core/lexer/lex.h"

void execute(const std::vector<lex::token>& tokens)
{
    // break tokens into `cmd` and `args` for the AO parser
    lex::token cmd = tokens.front();
    std::vector<lex::token> args = std::vector<lex::token>(tokens.begin() + 1, tokens.end());

    std::cout << cmd.type << " : [" << cmd.name << "]\n";
    for (std::vector<lex::token>::size_type i = 0; i < args.size(); i++)
        std::cout << args[i].type << " : [" << args[i].name << "]\n";
}
