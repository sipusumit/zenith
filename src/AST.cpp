#include "AST.h"

llvm::Value *IntegerAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module, std::shared_ptr<Env> env){
  return llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), value);
}

llvm::Value *StringAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module, std::shared_ptr<Env> env){
  return builder->CreateGlobalStringPtr(value);
}

llvm::Value *StatementsAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module, std::shared_ptr<Env> env){
  for(auto const& a : stmts){
    a->codegen(context, builder, module, env);
  }
  return nullptr;
}

llvm::Value *FunctionAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module, std::shared_ptr<Env> env){
  llvm::Function *f = module->getFunction(name);
  if(f != nullptr){
    std::cout << "Function already Defined " << name;
    exit(0);
  }

  llvm::FunctionType *functionTyInt = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context),false);
  f = llvm::Function::Create(functionTyInt,llvm::Function::ExternalLinkage,name,module);
  llvm::BasicBlock *bb =  llvm::BasicBlock::Create(*context, "entry", f);
  builder->SetInsertPoint(bb);
  body->codegen(context, builder, module, env);
  return f;
}

llvm::Value *FCall::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module, std::shared_ptr<Env> env){
  auto fn = module->getFunction(name);
  if(args.size() == 0)
    return builder->CreateCall(fn);
  std::vector<llvm::Value*> arguments;
  for(auto const& arg : args){
    arguments.push_back(arg->codegen(context, builder, module, env));
  }
  return builder->CreateCall(fn,arguments);
}

llvm::Value *ReturnAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module, std::shared_ptr<Env> env){
  return builder->CreateRet(value->codegen(context, builder, module, env));
}

llvm::Value *VarDeclAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module, std::shared_ptr<Env> env)
{
  llvm::Type *ty;
  switch (type)
  {
  case VType::INT:
    ty = builder->getInt32Ty();
    break;
  case VType::STRING:
    ty = builder->getInt8PtrTy();
    break;
  default:
    exit(1);
    break;
  }
  llvm::Value* v = builder->CreateAlloca(ty);
  llvm::Value* vl = value->codegen(context, builder, module, env);
  env->addVar(name,v);
  return builder->CreateStore(vl,v);
}

llvm::Value *IdentifierAST::codegen(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module, std::shared_ptr<Env> env)
{
  return env->getVal(name);
}
