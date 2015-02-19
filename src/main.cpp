#include <iostream>
#include "FS.hpp"
#include "DefaultLoop.hpp"
#include "common.hpp"
#include "Argument.hpp"

using namespace cpuv;
int main(int /*argc*/,char**){
  FS fs;
  std::vector<std::string> files {"/home/hesham/cpplay/test"};
  fs.watch(files,[](Argument<FS>& arg){
      static int i = 5;
      if(i-- > 0) {
      std::cout<<arg.fileName.first<<std::endl;
      std::cout<<arg.fileName.second<<std::endl;
      }else
      arg._this().unwatch(arg.fileName.first);
  });
  DefaultLoop::getInstance().run();
}
