#include "aopch.h"
#include "execute.h"
#include "ao.h"

#include "core/lexer/lex.h"
#include "core/settings/settings.h"

#include "strings/strings.h"
#include "console/console.h"

int execute(const std::vector<lex::token>& tokens)
{
    const std::vector<std::vector<lex::token>> preprocessed_tokens = preprocess_tokens(tokens);

    for (std::vector<std::vector<lex::token>>::size_type i = 0; i < preprocessed_tokens.size(); i++)
    {
        // break tokens into `cmd` and `args` for the AO parser
        lex::token cmd = preprocessed_tokens[i].front();
        std::vector<lex::token> args = std::vector<lex::token>(preprocessed_tokens[i].begin() + 1, preprocessed_tokens[i].end());

        std::function<void()> func = get_command_func(cmd.name);
        if (func != nullptr)
            func();

        else if (cmd.name == "exit" || cmd.name == "_exit" || cmd.name == "-c")
            return 0; // 0 means that the user wants to exit AO

        else if (cmd.name == "reload" || cmd.name == "-r" || cmd.name == "_refresh")
            return 2; // 2 means that the user wants to reload AO

        else if (cmd.type == lex::EXPR)
            std::cout << cmd.name << std::endl;

        else if (cmd.type == lex::STRING)
        {
            // trim string literals from start and end
            std::string trimmed_str = strings::trim(cmd.name, 1, 2);
            std::cout << trimmed_str << std::endl;
        }

        else if (cmd.name == ">" && cmd.type == lex::SYMBOL)
        {
            // _s stands for converted to strings
            std::string cmd_s = cmd.name;
            std::vector<std::string> args_s(args.size());
            std::transform(args.begin(), args.end(), args_s.begin(), [](const lex::token& token) { return token.name; });
            std::system((cmd_s + " " + strings::trim(strings::join("", args_s))).c_str());
        }

        else
            console::errors::runtime(cmd.name, "Command not found");
    }

    return 1; // 1 means that the user continue using AO
}

std::vector<std::vector<lex::token>> preprocess_tokens(const std::vector<lex::token>& tokens)
{
    std::vector<std::vector<lex::token>> all_tokens;
    std::vector<lex::token> subtokens;

    for (std::vector<lex::token>::size_type i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].type == lex::COMMENT || tokens[i].type == lex::EOL)
        {
            if (subtokens.size() > 0)
            {
                all_tokens.push_back(subtokens);
                subtokens.clear();
            }

            break;
        }

        else if (tokens[i].type == lex::SEMICOLON)
        {
            all_tokens.push_back(subtokens);
            subtokens.clear();
        }

        else
            subtokens.push_back(tokens[i]);
    }

    return all_tokens;
}
