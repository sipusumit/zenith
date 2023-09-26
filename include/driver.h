#pragma once

#include<fstream>

#include "config.h"
#include "parser.h"

#include <memory>

class Driver
{
private:
  ZConfig config;
  Lexer lexer;
  Parser parser;
public:
  Driver(ZConfig config);
  void compile();
};
