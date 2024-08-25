#include "aopch.h"
#include "lex.h"

#include "strings/strings.h"

void lex::math(std::string& expression)
{
    // std::regex re(R"([\+\-\*/]+|\d*\.?\d+|[\(\)])");
    // std::regex re(R"([-+*/()]+|\d+(?:_\d+)*\.?\d*)");
    // expression = strings::replace_all(expression, "_", "");
    // std::vector<std::string> toks = this->tokenizer(expression, re);

    // for (std::vector<std::string>::size_type i = 0; i < toks.size(); i++)
    // {
    //     std::cout << "(" << toks[i] << ")\n";
    // }
}
