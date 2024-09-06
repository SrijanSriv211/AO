#include "aopch.h"
#include "lex.h"

#include "strings/strings.h"
#include "console/console.h"
#include "tinyexpr/tinyexpr.h"

std::string lex::math(const std::string& expression)
{
    // preprocess expression
    std::string expr = strings::replace_all(expression, "_", "");
    expr = strings::replace_all(expr, " ", "");

    // use tinyexpr to evaluate the expression
    double result = te_interp(expr.c_str(), 0);
    std::string result_s = std::format("{}", result); // _s means converted to string

    if (result_s == "nan")
        return expr;

    else if (result_s == "inf")
        console::errors::runtime("∞", "Division by zero");

    return result_s;
}
