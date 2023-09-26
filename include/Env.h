#pragma once

#include<map>
#include "AST.h"

class Env{
private:
  std::map<std::string, llvm::Value*> variables;
public:
  bool hasVar(std::string name);
};