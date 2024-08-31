#pragma once

#include "core/lexer/lex.h"

std::vector<std::vector<lex::token>> preprocess_tokens(const std::vector<lex::token>& tokens);
std::function<void()> get_command_func(const std::string& cmd);
int execute(const std::vector<lex::token>& tokens);
