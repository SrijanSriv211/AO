#pragma once

class lex
{
public:
    enum token_type
    {
        EOL = 0, // END OF LINE
        UNKNOWN, // 1
        COMMENT, // 2
        AMPERSAND, // 3
        WHITESPACE, // 4
        IDENTIFIER, // 5
        SEMICOLON, // 6
        HIDDEN, // 7
        STRING, // 8
        SYMBOL, // 9
        FLAGS, // 10
        BOOL, // 11
        EXPR // 12
    };

    struct token
    {
        std::string name;
        token_type type;
    };

public:
    // lex(const std::string& str, const bool& break_at_error=true, const bool& unescape_strings=true, const bool& get_env_var=true, const bool& do_math=true);
    lex(const std::string& str, const bool& break_at_error=true, const bool& eval_tokens=true);
    void print_error();

public:
    std::vector<token> tokens;
    std::string error;

private:
    void assign_token_type(const std::vector<std::string>& toks);
    std::vector<std::string> tokenizer(const std::string& str, const std::regex& re);
    std::vector<token> merge_tokens(const std::vector<token>& toks);

    bool is_valid_string(const std::string& str);
    void unescape_string(std::string& str);

    bool is_env_var_string(const std::string& str);
    token get_env_var_val(const std::string& str);

    void math(std::string& expression);

private:
    // bool do_math;
    // bool get_env_var;
    bool eval_tokens;
    bool break_at_error;
    // bool unescape_strings;

    std::map<std::string, std::string> escape_chars = {};

    std::regex r_str = std::regex(R"(\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'|`(?:\\.|[^'\\])*`)");
    std::regex r_id = std::regex(R"([-_/.a-zA-Z]+)");
    std::regex r_math = std::regex(R"(\d+(?:_\d+)*\.?\d*|[-+*/()]+?)");
    std::regex r_symbol = std::regex(R"([(),;&?@!:>])");
    std::regex r_extra = std::regex(R"([ ]+|#.*|.+)"); // comments and white spaces
    // std::regex math_re = std::regex(R"(\(*\d+(?:[_\d]*)\)*(?:\s*[-+*/]\s*\(*\d+(?:[_\.\d]*)\)*)*)");
    // std::regex str_re = std::regex(R"(\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'|`(?:\\.|[^'\\])*`)");
    // std::regex identifier_re = std::regex(R"([\w\d_\-.+*/]+)");
    // std::regex symbol_re = std::regex(R"([(),;?@!:>]+)");
    // std::regex extras_re = std::regex(R"([ ]+|#.*|.+)"); // comments and white spaces
};
