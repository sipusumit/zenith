#include "Env.h"

bool Env::hasVar(std::string name)
{
  if(variables.count(name))
    return true;
  if(parent != nullptr)
    return parent->hasVar(name);
  return false;
}

void Env::addVar(std::string name, llvm::Value * value)
{
  variables[name] = value;
}

llvm::Value *Env::getVal(std::string name)
{
  if(hasVar(name))
    return variables[name];
  
  if(parent != nullptr)
    return parent->getVal(name);
  
  return nullptr;
}
