#pragma once

#include "lexer.h"
#include "AST.h"

class Parser
{
private:
  std::string file, src;
  Lexer lexer;
  std::vector<Token> tokens;

  Token eat();
  Token eat(TType type);

  std::shared_ptr<AST> parse_stmt();
  std::shared_ptr<AST> var_or_function_decl();
  std::shared_ptr<FunctionAST> function_decl(std::string type, std::string name);
  std::shared_ptr<VarDeclAST> var_decl(TType type, std::string name);
  std::shared_ptr<FCall> parse_fcall();
  std::shared_ptr<ReturnAST> parse_return();
  std::shared_ptr<StringAST> parse_string();
  std::shared_ptr<IntegerAST> parse_int();
public:
  Parser(){};
  Parser(std::string file, std::string src): file(file), src(src){
    lexer = Lexer(file, src);

    Token t = lexer.getNextToken();
    while(t.type != TType::TEOF){
      if(t.type == TType::WS || t.type == TType::NL){
        t = lexer.getNextToken();
        continue;
      }
      tokens.push_back(t);
      t = lexer.getNextToken();
    }
    tokens.push_back(t);
  };
  std::vector<std::shared_ptr<AST>> parse();
};