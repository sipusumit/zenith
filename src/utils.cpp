#include "utils.h"

std::vector<std::string> split(const std::string &s, char delim){
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delim)){
    result.push_back(item);
  }
  return result;
}

void unexpected(std::string file, std::string line, Token t){
    std::cout << file << ":" << t.line << ":" << t.col << ": ";
    std::cout << "Unexpected Token: " << t.value << std::endl;
    std::cout << t.line << " |  " << line << "\n"
          << std::string(std::to_string(t.line).length(), ' ') << " | ";
    std::cout << std::string(t.col, ' ') << "^";
    exit(1);
}

std::string parseString(std::string str){
  if(str.size() > 2 && str[0] == '"' && str[str.size() -1 ] == '"'){
    str = str.substr(1, str.size()-2);
  }

  std::regex r(R"(\\(.))");
  //TODO: replace escape characters to real escape characters
  str = std::regex_replace(str, r, "$1");
  return str;
}