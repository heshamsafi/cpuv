#include <iostream>
#include "FS.hpp"
#include "DefaultLoop.hpp"
#include "common.hpp"
#include "Argument.hpp"

using namespace cpuv;
int main(int /*argc*/,char** argv){
  FS fs;
  fs.fileName(argv[1]).open([](Argument<FS>& arg){
    if(arg.status == Status::ERROR) ERR(arg.errorMsg);
    else
    arg._this().read([](Argument<FS>& arg){
	switch(arg.status){
	case Status::ERROR:
	  ERR(arg.errorMsg);
	case Status::END_OF_FILE:
	  arg._this().close();
	break;
	default:
	  //arg._this().writeSync(STDOUT_FILENO, arg.buffer().get<char*>(), arg.buffer().size);
	  arg._this().write(arg.buffer().get<char*>()
	      ,arg.buffer().size, [](Argument<FS>& arg){
	    arg._this().read();
	  },nullptr,STDOUT_FILENO);
	  //arg._this().read();
	}
    }); 
  });
  DefaultLoop::getInstance().run();
}
