#include "aopch.h"
#include "lex.h"

#include "strings/strings.h"
#include "tinyexpr/tinyexpr.h"

std::string lex::math(const std::string& expression)
{
    // preprocess expression
    std::string expr = strings::replace_all(expression, "_", "");
    expr = strings::replace_all(expr, " ", "");

    // use tinyexpr to evaluate the expression
    double result = te_interp(expr.c_str(), 0);
    return std::format("{}", result);
}