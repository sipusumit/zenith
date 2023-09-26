#include "Env.h"

bool Env::hasVar(std::string name)
{
  if(variables.count(name))
    return true;
  return false;
}
