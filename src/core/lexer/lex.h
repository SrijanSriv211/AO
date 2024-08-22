#pragma once

class lex
{
public:
    enum token_type
    {
        EOL = 0, // END OF LINE
        UNKNOWN, // 1
        COMMENT, // 2
        WHITESPACE, // 3
        IDENTIFIER, // 4
        SEMICOLON, // 5
        HIDDEN, // 6
        STRING, // 7
        SYMBOL, // 8
        FLAGS, // 9
        BOOL, // 10
        EXPR // 11
    };

    struct token
    {
        std::string name;
        token_type type;
    };

public:
    lex(const std::string& code, const bool& break_at_error=true, const bool& unescape_strings=true, const bool& get_env_var=true, const bool& do_math=true);
    void print_error();

public:
    std::vector<token> tokens;
    std::string error;

private:
    void parse(const std::vector<std::string>& toks);
    std::vector<std::string> tokenizer(const std::string& str, const std::regex& re);
    std::vector<token> reduce_toks(const std::vector<token>& toks);

private:
    bool do_math;
    bool get_env_var;
    bool break_at_error;
    bool unescape_strings;
    std::map<std::string, std::string> escape_chars = {};
    std::regex math_re = std::regex(R"(\(*\d+(?:[_\d]*)\)*(?:\s*[-+*/]\s*\(*\d+(?:[_\.\d]*)\)*)*)");
    std::regex str_re = std::regex(R"(\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'|`(?:\\.|[^'\\])*`)");
    std::regex identifier_re = std::regex(R"([\w\d_\-.+*/]+)");
    std::regex symbol_re = std::regex(R"([(),;?@!:>]+)");
    std::regex extras_re = std::regex(R"([ ]+|#.*|.+)"); // comments and white spaces
};
