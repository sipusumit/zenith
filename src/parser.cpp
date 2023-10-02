#include "parser.h"

Token Parser::eat(){
  Token t = tokens[0];
  tokens.erase(tokens.begin());
  return t;
}

template <typename Enumeration>
auto as_integer(Enumeration const value)-> typename std::underlying_type<Enumeration>::type{
  return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

Token Parser::eat(TType type){
  Token t = eat();
  if(t.type != type){
    unexpected(file, split(src, '\n')[t.line - 1], t);
  }
  return t;
}




std::vector<std::shared_ptr<AST>> Parser::parse(){
  std::vector<std::shared_ptr<AST>> stmts;
  while(tokens[0].type != TType::TEOF){
    stmts.push_back(std::move(parse_stmt()));
  }
  return stmts;
}

std::shared_ptr<AST> Parser::parse_stmt(){
  std::shared_ptr<AST> a;
  switch (tokens[0].type){
    case TType::T_INT:
    case TType::T_STRING:
      a =  var_or_function_decl();
      break;
    case TType::IDENTIFIER:
      a =  parse_ident_or_fcall();
      break;
    case TType::RETURN:
      a =  parse_return();
      break;
    case TType::STRING:
      a =  parse_string();
      break;
    case TType::INT:
      a =  parse_int();
      break;
    case TType::TEOF:
      std::cout << "Unexpected end of file\n";
      exit(0);
    default:
      unexpected(file,split(src, '\n')[tokens[0].line - 1],tokens[0]);
      break;
  }
  return a;
};


// var_or_func_decl
//   : type IDENTIFIER ( ) { stmt }
//   | type IDENTIFIER = expr
//   ;
std::shared_ptr<AST> Parser::var_or_function_decl(){
  Token t = eat();
  std::string name = eat(TType::IDENTIFIER).value;
  switch (tokens[0].type)
  {
  case TType::EQUALS: // var
    return var_decl(t.type, name);
    break;
  case TType::LPAREN: // func
    return function_decl(t.value, name);
    break;
  default:
    unexpected(file,split(src, '\n')[tokens[0].line - 1],tokens[0]);
    exit(1);
    break;
  }
  
}

std::shared_ptr<FunctionAST> Parser::function_decl(std::string type, std::string name){
  eat(TType::LPAREN); // (
  // TODO: add parameters
  eat(TType::RPAREN); // )
  eat(TType::LBRACE); // {
  std::vector<std::shared_ptr<AST>> body;
  while(tokens[0].type != TType::RBRACE){ // }
    body.push_back(std::move(parse_stmt()));
  }
  eat(TType::RBRACE); // }
  StatementsAST s = StatementsAST(body);
  return std::make_shared<FunctionAST>(FunctionAST(name, type, std::make_shared<StatementsAST>(s)));
}

std::shared_ptr<VarDeclAST> Parser::var_decl(TType type, std::string name)
{
  eat(TType::EQUALS); // =
  VType t;
  switch (type)
  {
  case TType::T_INT:
    t = VType::INT;
    break;
  case TType::T_STRING:
    t = VType::STRING;
    break;
  default:
    std::cout << "Unknown variable type ";
    exit(1);
    break;
  }
  std::shared_ptr<AST> value = parse_stmt();
  eat(TType::SCOLON); // ;
  return std::make_shared<VarDeclAST>(t,name,value);
}
// ident_or_fcall
//  : IDENTIFIER 
//  | IDENTIFIER ( ) ;
std::shared_ptr<AST> Parser::parse_ident_or_fcall(){
  std::string name = eat(TType::IDENTIFIER).value;
  if(tokens[0].type != TType::LPAREN)
    return std::make_shared<IdentifierAST>(name);
  return parse_fcall(name);
}

// fcall
//   : IDENTIFIER ( ) ;
//   ;
std::shared_ptr<FCall> Parser::parse_fcall(std::string name){
  eat(TType::LPAREN); // (
  // ARGS
  std::vector<std::shared_ptr<AST>> args;
  while(tokens[0].type != TType::RPAREN){
    args.push_back(parse_stmt());
    // if(tokens[0]) comma seperated args
  }
  eat(TType::RPAREN); // )
  eat(TType::SCOLON); // ;
  return std::make_shared<FCall>(name, args);
}

std::shared_ptr<ReturnAST> Parser::parse_return(){
  eat(TType::RETURN); // return
  // Token t = eat();
  std::shared_ptr<ReturnAST> r;
  // if(t.type == TType::INT){
  //   IntegerAST v = IntegerAST(std::stoi(t.value));
  //   r = std::make_shared<ReturnAST>(std::make_shared<IntegerAST>(v));
  // }
  // else if(t.type == TType::STRING){
  //   StringAST v = StringAST(t.value);
  //   r = std::make_shared<ReturnAST>(std::make_shared<StringAST>(v));
  // }else{
  //   std::cout << "Unexpected Return Type";
  //   exit(1);
  // }
  r = std::make_shared<ReturnAST>(parse_stmt());
  eat(TType::SCOLON); //;
  return r;
}

std::shared_ptr<StringAST> Parser::parse_string(){
  return std::make_shared<StringAST>(eat(TType::STRING).value);
}

std::shared_ptr<IntegerAST> Parser::parse_int(){
  return std::make_shared<IntegerAST>(eat(TType::INT).value);
}