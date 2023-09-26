#pragma once

#include <string>
#include <regex>
#include <sstream>
#include <iostream>

#include "fifo_map.hpp"

using namespace nlohmann;

enum class VType{
  INT,
  STRING
};

enum class TType{
  T_INT,
  T_STRING,
  RETURN,

  INT,
  STRING,

  IDENTIFIER,
  RPAREN,
  LPAREN,
  RBRACE,
  LBRACE,
  SCOLON,
  EQUALS,

  WS,
  NL,
  TEOF,
};

extern fifo_map<std::string,TType> PATTERNS;

typedef struct {
  TType type;
  std::string value;
  int line, col;
} Token;

#include "utils.h"

class Lexer{
private:
  std::string src;
  std::string file;
  int at, line, col;
  void error(std::string s);

public:
  Lexer(std::string file, std::string src): file(file), src(src),at(0),line(1), col(1){};
  Lexer(): line(1), col(1){};
  Token getNextToken();
};