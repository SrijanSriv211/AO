#include "aopch.h"
#include "lex.h"

#include "strings/strings.h"

void lex::parse(const std::vector<std::string>& toks)
{
    std::vector<lex::token> tokens;
    std::string tok;

    for (std::vector<std::string>::size_type i = 0; i < toks.size(); i++)
    {
        tok = toks[i];

        if (strings::is_empty(tok))
        {
            tokens.push_back({tok, lex::WHITESPACE});
            tok.clear();
        }

        // COMMENT
        else if (tok.starts_with('#'))
        {
            tokens.push_back({tok, lex::COMMENT});
            tok.clear();
        }

        else if (tok == ";")
        {
            tokens.push_back({tok, lex::SEMICOLON});
            tok.clear();
        }

        else if (strings::any(tok, {">", "@", "!", "?", ":"}, true))
        {
            tokens.push_back({tok, lex::SYMBOL});
            tok.clear();
        }

        else if (tok.starts_with("\"") || tok.starts_with("'") || tok.starts_with("`"))
        {
            if (tok.length() == 1)
            {
                this->error = "unexpected end of tokens after " + std::string(1, tok.front());

                if (this->break_at_error)
                {
                    this->print_error();
                    break;
                }
            }

            else if (tok.front() != tok.back())
            {
                this->error = "missing terminating " + std::string(1, tok.front()) + " character";

                if (this->break_at_error)
                {
                    this->print_error();
                    break;
                }
            }

            if (this->unescape_strings)
            {
                for (const auto& [key, val] : this->escape_chars)
                {
                    if (tok.find(key) != std::string::npos)
                        tok = strings::replace_all(tok, key, val);
                }
            }
            lex::token str_token = {tok, lex::STRING};

            // check for env vars
            if (tok.front() == '`' && tok.size() >= 3 && this->get_env_var)
            {
                // it will have at least 3 chars '`' and the env char,
                // therefore no need to check if tok is greater than 3 chars or not.
                const std::string env_var_name = tok.substr(1, tok.size() - 2);
                const char* env_var_val = std::getenv(env_var_name.c_str());

                if (env_var_val != nullptr)
                {
                    // push it as an identifier since it won't be wrapped inside any string literals
                    //*NOTE: It may change and become more dynamic, so if the env val is an int,
                    //* then the token type will be an int. but that's for future, not now.
                    str_token = {env_var_val, lex::IDENTIFIER};
                }
            }

            tokens.push_back(str_token);
            tok.clear();
        }

        // check for math expressions
        else if (std::regex_match(tok, this->math_re))
        {
            std::string expr_token = tok;

            //TODO: Implement a math engine to evaluate math exprs.
            if (this->do_math)
            {
                expr_token = strings::replace_all(tok, "_", "");
            }

            tokens.push_back({expr_token, lex::EXPR});
            tok.clear();
        }

        else
        {
            lex::token_type type;

            if (tok == "true" || tok == "false")
                type = lex::BOOL;

            else if (tok.starts_with("-") || tok.starts_with("/"))
                type = lex::FLAGS;

            else if (tok.starts_with("_"))
                type = lex::HIDDEN;

            else
                type = lex::IDENTIFIER;

            tokens.push_back({tok, type});
            tok.clear();
        }
    }

    this->tokens = this->reduce_toks(tokens);
}

// reduce tokens by combining just succeeding tokens with same type into a single token
std::vector<lex::token> lex::reduce_toks(const std::vector<lex::token>& toks)
{
    std::vector<lex::token> new_tokens;
    lex::token_type tok_type;
    std::string tok_name;

    for (std::vector<lex::token>::size_type i = 0; i < toks.size(); i++)
    {
        // for now only combine identifiers. maybe in future i'll change it and make it work in general.
        if (toks[i].type == toks[i+1].type && toks[i].type == lex::IDENTIFIER)
        {
            tok_name += toks[i].name + toks[i+1].name;
            tok_type = toks[i].type;
            i++;
        }

        else
        {
            if (tok_name != "")
            {
                // push the previous common tokens to the array
                new_tokens.push_back({tok_name, tok_type});

                // clear the concatenated tok_name and tok_type
                tok_name.clear();
                tok_type = UNKNOWN;
            }

            // push the current tokens to the array
            new_tokens.push_back({toks[i].name, toks[i].type});
        }
    }

    return new_tokens;
}
