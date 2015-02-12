#include <iostream>
#include "FS.hpp"
#include "DefaultLoop.hpp"
#include "common.hpp"
#include "Argument.hpp"
#include <memory>
  
int main(int /*argc*/,char** argv){
  FS fs;
  std::string fileName(argv[1]);
  fs.fileName(fileName).open([](cpuv::Argument* arg){
    std::unique_ptr<cpuv::Argument> argPtr(arg);
    if(arg->status == Status::ERROR) ERR(arg->errorMsg);
    char buffer[1024];
    arg->fs->buffer = buffer;
    arg->fs->read([](cpuv::Argument* arg){
	if(arg->status == Status::ERROR) {
	  ERR(arg->errorMsg);
	  arg->fs->close(arg);
	} else if(arg->status == Status::END_OF_FILE) arg->fs->close(arg);
	else {
	  std::cout<<arg->fs->buffer;
	  arg->fs->read();
	}
    }); 
  });
  DefaultLoop::getInstance().run();
}
