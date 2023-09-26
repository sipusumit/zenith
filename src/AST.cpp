#include "AST.h"

llvm::Value *IntegerAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module){
  return llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), value);
}

llvm::Value *StringAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module){
  return builder->CreateGlobalStringPtr(value);
}

llvm::Value *StatementsAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module){
  for(auto const& a : stmts){
    a->codegen(context, builder, module);
  }
  return nullptr;
}

llvm::Value *FunctionAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module){
  llvm::Function *f = module->getFunction(name);
  if(f != nullptr){
    std::cout << "Function already Defined " << name;
    exit(0);
  }

  llvm::FunctionType *functionTyInt = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context),false);
  f = llvm::Function::Create(functionTyInt,llvm::Function::ExternalLinkage,name,module);
  llvm::BasicBlock *bb =  llvm::BasicBlock::Create(*context, "entry", f);
  builder->SetInsertPoint(bb);
  body->codegen(context, builder, module);
  return f;
}

llvm::Value *FCall::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module){
  auto fn = module->getFunction(name);
  if(args.size() == 0)
    return builder->CreateCall(fn);
  std::vector<llvm::Value*> arguments;
  for(auto const& arg : args){
    arguments.push_back(arg->codegen(context, builder, module));
  }
  return builder->CreateCall(fn,arguments);
}

llvm::Value *ReturnAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module){
  return builder->CreateRet(value->codegen(context, builder, module));
}