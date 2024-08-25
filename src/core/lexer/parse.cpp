#include "aopch.h"
#include "lex.h"

#include "strings/strings.h"

void lex::assign_token_type(const std::vector<std::string>& toks)
{
    std::vector<lex::token> tokens;
    lex::token tok = {};
    std::string str;

    for (std::vector<std::string>::size_type i = 0; i < toks.size(); i++)
    {
        str = toks[i];

        if (strings::is_empty(str))
            tok = {str, lex::WHITESPACE};

        else if (str.starts_with('#'))
            tok = {str, lex::COMMENT};

        else if (str == ";")
            tok = {str, lex::SEMICOLON};

        else if (str == "&")
            tok = {str, lex::AMPERSAND};

        else if (str.starts_with("_"))
            tok = {str, lex::HIDDEN};

        else if (strings::any(str, {"true", "false"}, true))
            tok = {str, lex::BOOL};

        else if (strings::any(str, {">", "@", "!", "?", ":"}, true))
            tok = {str, lex::SYMBOL};

        else if (str.starts_with("\"") || str.starts_with("'") || str.starts_with("`"))
        {
            if (!this->is_valid_string(str))
                break;

            // else if (this->unescape_strings)
            //     this->unescape_string(str);

            // tok = this->is_env_var_string(str) ? this->get_env_var_val(str) : lex::token({str, lex::STRING});
            tok = {str, lex::STRING};
        }

        // check for math expressions
        else if (strings::only(str, {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "+", "*", "/", "(", ")", "_", ".", " "}))
            tok = {str, lex::EXPR};

        else if (str.starts_with("-") || str.starts_with("/"))
            tok = {str, lex::FLAGS};

        else
            tok = {str, lex::IDENTIFIER};

        tokens.push_back(tok);
        str.clear();
    }

    this->tokens = this->merge_tokens(tokens);
}

std::vector<lex::token> lex::merge_tokens(const std::vector<lex::token>& toks)
{
    std::vector<lex::token> tokens;
    lex::token_type tok_type = lex::UNKNOWN;
    std::string tok_name = "";

    for (std::vector<lex::token>::size_type i = 0; i < toks.size(); i++)
    {
        if (toks[i].type == lex::EXPR && toks[i+1].type == lex::EXPR)
        {
            tok_name += strings::is_empty(tok_name) ? toks[i].name + toks[i+1].name : toks[i+1].name;
            tok_type = lex::EXPR;
        }

        else if (toks[i].type == lex::IDENTIFIER && toks[i+1].type == lex::IDENTIFIER)
        {
            tok_name += strings::is_empty(tok_name) ? toks[i].name + toks[i+1].name : toks[i+1].name;
            tok_type = lex::IDENTIFIER;
        }

        // else if (toks[i].type == lex::STRING && toks[i+1].type == lex::AMPERSAND)
        // {
        //     tok_name = toks[i].name;
        //     tok_type = lex::IDENTIFIER;
        // }

        else if (!strings::is_empty(tok_name) && tok_type != lex::UNKNOWN)
        {
            // push the previous common tokens to the array
            tokens.push_back({tok_name, tok_type});

            // clear the concatenated tok_name and tok_type
            tok_name.clear();
            tok_type = UNKNOWN;
        }

        // push the current tokens to the array
        else
            tokens.push_back({toks[i].name, toks[i].type});

    }

    return tokens;
}
