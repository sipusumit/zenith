#pragma once

#include <map>
#include <memory>
#include <llvm/IR/Value.h>

class Env{
private:
  std::map<std::string, llvm::Value*> variables;
  std::shared_ptr<Env> parent;
public:
  Env(){parent = nullptr;};
  Env(std::shared_ptr<Env> parent): parent(parent){};
  bool hasVar(std::string name);
  void addVar(std::string name, llvm::Value* value);
  llvm::Value* getVal(std::string name);
};