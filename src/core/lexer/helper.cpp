#include "aopch.h"
#include "lex.h"

#include "strings/strings.h"

bool lex::is_valid_string(const std::string& str)
{
    if (str.length() == 1)
    {
        this->error = "unexpected end of tokens after " + std::string(1, str.front());

        if (this->break_at_error)
        {
            this->print_error();
            return false;
        }
    }

    else if (str.front() != str.back())
    {
        this->error = "missing terminating " + std::string(1, str.front()) + " character";

        if (this->break_at_error)
        {
            this->print_error();
            return false;
        }
    }

    return true;
}

void lex::unescape_string(std::string& str)
{
    for (const auto& [key, val] : this->escape_chars)
    {
        if (str.find(key) != std::string::npos)
            str = strings::replace_all(str, key, val);
    }
}

bool lex::is_env_var_string(const std::string& str)
{
    return str.front() == '`' && str.size() >= 3 && this->get_env_var;
}

lex::token lex::get_env_var_val(const std::string& str)
{
    // it will have at least 3 chars '`' and the env char,
    // therefore no need to check if str is greater than 3 chars or not.
    const std::string env_var_name = str.substr(1, str.size() - 2);
    const char* env_var_val = std::getenv(env_var_name.c_str());

    if (env_var_val != nullptr)
    {
        // push it as an identifier since it won't be wrapped inside any string literals
        //*NOTE: It may change and become more dynamic, so if the env val is an int,
        //* then the token type will be an int. but that's for future, not now.
        return {env_var_val, lex::IDENTIFIER};
    }

    return {str, lex::STRING};
}
