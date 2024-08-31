#include "aopch.h"
#include "execute.h"

#include "core/lexer/lex.h"
#include "strings/strings.h"

int execute(const std::vector<lex::token>& tokens)
{
    const std::vector<std::vector<lex::token>> preprocessed_tokens = preprocess_tokens(tokens);

    for (std::vector<std::vector<lex::token>>::size_type i = 0; i < preprocessed_tokens.size(); i++)
    {
        // break tokens into `cmd` and `args` for the AO parser
        lex::token cmd = preprocessed_tokens[i].front();
        std::vector<lex::token> args = std::vector<lex::token>(preprocessed_tokens[i].begin() + 1, preprocessed_tokens[i].end());

        if (cmd.type == lex::FLAG && get_command_func(cmd.name) != nullptr)
            get_command_func(cmd.name)();

        else if (((cmd.name == "-exit" || cmd.name == "-c") && cmd.type == lex::FLAG))
            return 0;

        else if (cmd.type == lex::STRING || cmd.type == lex::EXPR)
            std::cout << cmd.name << "\n\n";

        else
        {
            // _s stands for converted to strings
            std::string cmd_s = cmd.name;
            std::vector<std::string> args_s(args.size());
            std::transform(args.begin(), args.end(), args_s.begin(), [](const lex::token& token) { return token.name; });
            system((cmd_s + " " + strings::join("", args_s)).c_str());
        }
    }

    return 1;
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
