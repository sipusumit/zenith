#pragma once
#include <string>

#ifndef O_EXT
#define O_EXT "out"
#endif

class ZConfig
{
public:
  bool compileOnly;
  std::string outputFile, inputFile;


  ZConfig(){
    this->compileOnly = false;
    this->outputFile = "a." O_EXT;
  };
  ZConfig(bool compileOnly, std::string outputFile, std::string inputFile): compileOnly(compileOnly), outputFile(outputFile), inputFile(inputFile){};
};
