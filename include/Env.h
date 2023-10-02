#pragma once

#include <map>
#include <memory>
#include <llvm/IR/Value.h>

typedef struct {
  llvm::Value* value;
  llvm::Type* type;
} var_t;

class Env{
private:
  std::map<std::string, var_t> variables;
  std::shared_ptr<Env> parent;
public:
  Env(){parent = nullptr;};
  Env(std::shared_ptr<Env> parent): parent(parent){};
  bool hasVar(std::string name);
  void addVar(std::string name, var_t value);
  var_t getVal(std::string name);
};