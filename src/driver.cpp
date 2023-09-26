#include "driver.h"

Driver::Driver(ZConfig config){
  this->config = config;
  std::ifstream t(config.inputFile);
  std::stringstream buffer;
  buffer << t.rdbuf();
  parser = Parser(config.inputFile , buffer.str());
  // lexer = Lexer(config.inputFile , buffer.str());
  t.close();
}

void setupstd(llvm::LLVMContext *context, llvm::IRBuilder<> *builder, llvm::Module *module){
  module->getOrInsertFunction("printf",
                              llvm::FunctionType::get(builder->getInt32Ty(),
                                builder->getInt8Ty()->getPointerTo(),
                                true
                              ));
}

void Driver::compile(){
  // Token t = lexer.getNextToken();
  // while (t.type != TType::TEOF)
  // {
  //   std::cout << "\n";
  //   std::cout << t.line << ":" << t.col << ": " << t.value << "\n";
  //   t = lexer.getNextToken();
  // }

  std::vector<std::shared_ptr<AST>> stmts = parser.parse();

  llvm::LLVMContext context = llvm::LLVMContext();
  std::shared_ptr<llvm::Module> module = std::make_shared<llvm::Module>("test", context);
  llvm::IRBuilder<> builder(context);
  setupstd(&context, &builder, module.get());

  for(auto const& s : stmts){
    s->codegen(&context, &builder, module.get());
  }
  module->print(llvm::outs(), nullptr);
}