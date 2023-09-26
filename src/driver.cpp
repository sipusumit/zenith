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
  module->setSourceFileName(config.inputFile);
  llvm::IRBuilder<> builder(context);
  std::shared_ptr<Env> env = std::make_shared<Env>();
  // setup standard functions
  setupstd(&context, &builder, module.get());

  for(auto const& s : stmts){
    s->codegen(&context, &builder, module.get(), env);
  }

  if(config.outputFile == "a." O_EXT){
    module->print(llvm::outs(), nullptr);
    llvm::outs().flush();
  }

  // SAVE IR TO FILE ===============================================
  std::error_code EC;
  llvm::raw_fd_ostream OS(config.outputFile, EC);
  if(EC){
    std::cout << "Error creating output stream";
  }
  std::cout << "Writing to file " << config.outputFile << "\n";
  module->print(OS,nullptr);
  OS.flush();
  // ===============================================================

  // Compile to executable ==================================================
  std::cout << "Compiling " << config.outputFile << " to a." O_EXT << "\n";
  system(std::format("clang {}", config.outputFile).c_str());
  // ========================================================================
}