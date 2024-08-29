#pragma once

#include "core/lexer/lex.h"
#include "argparse/argparse.h"

void unrecognized_argument_error(const std::string& err);
void init_folders();
void setup();

int exec_parsed_args(argparse& parser, const std::vector<argparse::parsed_argument>& parsed_args);
int take_entry(const std::vector<std::string> args);
