#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>

class AST{
public:
  virtual llvm::Value *codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module){
    std::cout << "BASE \n";
    return nullptr;
  };
};

class IntegerAST : public AST{
private:
  int value;
public:
  IntegerAST(int value): value(value){};
  virtual llvm::Value *codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module) override;
};

class StringAST : public AST{
private:
  std::string value;
public:
  StringAST(std::string value): value(value){};
  virtual llvm::Value *codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module) override;
};

class StatementsAST : public AST{
private:
  std::vector<std::shared_ptr<AST>> stmts;
public:
  StatementsAST(std::vector<std::shared_ptr<AST>> stmts): stmts(std::move(stmts)){};
  virtual llvm::Value *codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module) override;
};

class FunctionAST : public AST{
private:
  std::string type, name;
  std::shared_ptr<StatementsAST> body;
public:
  FunctionAST(std::string name, std::string type, std::shared_ptr<StatementsAST> body): name(name), type(type), body(std::move(body)){};
  virtual llvm::Value *codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module) override;
};

class FCall : public AST{
private:
  std::string name;
  std::vector<std::shared_ptr<AST>> args;
public:
  FCall(std::string name): name(name){};
  FCall(std::string name, std::vector<std::shared_ptr<AST>> args): name(name), args(args){};
  virtual llvm::Value *codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module) override;
};

class ReturnAST : public AST{
private:
  std::shared_ptr<AST> value;
public:
  ReturnAST(){value = std::make_shared<IntegerAST>(0);};
  ReturnAST(std::shared_ptr<AST> value): value(std::move(value)){};
  virtual llvm::Value *codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module) override;
};