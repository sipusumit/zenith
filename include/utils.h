#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <regex>

#include "lexer.h"

std::vector<std::string> split(const std::string &s, char delim);

void unexpected(std::string file, std::string line, Token t);

std::string parseString(std::string str);