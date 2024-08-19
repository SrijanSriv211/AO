#include "aopch.h"
#include "lex.h"

#include "console/console.h"

lex::lex(const std::string& code, const bool& break_at_error, const bool& unescape_strings)
{
    this->escape_chars = {
        {"\\\\", "\\"},
        {"\\\"", "\""},
        {"\\'", "'"},
        {"\\n", "\n"},
        {"\\n", "\n"},
        {"\\0", "\0"},
        {"\\t", "\t"},
        {"\\r", "\r"},
        {"\\b", "\b"},
        {"\\a", "\a"},
        {"\\f", "\f"}
    };

    this->break_at_error = break_at_error;
    this->unescape_strings = unescape_strings;
    std::regex re(R"(\(*\d+(?:[_\d]*)\)*(?:\s*[-+*/]\s*\(*\d+(?:[_\.\d]*)\)*)*|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'|[\w\d_\-.+*/]+|[(),;?@!:>]|[ ]+|#.*|.+)");

    std::vector<std::string> toks = this->tokenizer(code, re);
    this->parse(toks);
    // push EOL in tokens just to make sure that `tokens.size()` is not zero.
    // because it will be used in rendering by `readf`.
    tokens.push_back({"", lex::EOL});
}

std::vector<std::string> lex::tokenizer(const std::string& str, const std::regex& re)
{
    std::vector<std::string> toks;

    // https://stackoverflow.com/a/73927301/18121288
    for (std::sregex_iterator it{str.begin(), str.end(), re}, end{}; it != end; ++it)
        toks.push_back(it->str());

    return toks;
}

void lex::print_error()
{
    console::print(this->error, console::color::BLACK, console::color::LIGHT_RED);
}
