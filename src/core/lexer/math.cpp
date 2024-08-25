#include "aopch.h"
#include "lex.h"

#include "strings/strings.h"

std::string lex::math(const std::string& expression)
{
    std::string expr = strings::replace_all(expression, "_", "");
    expr = strings::replace_all(expr, " ", "");

    std::regex re(R"([-+*/()]+?|\d*\.?\d+)");
    std::vector<std::string> toks = this->tokenizer(expr, re);

    // for (std::vector<std::string>::size_type i = 0; i < toks.size(); i++)
    // {
    //     std::cout << "(" << toks[i] << ")\n";
    // }

    return expr;
}
