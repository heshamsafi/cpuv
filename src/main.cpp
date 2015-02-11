#include <iostream>
#include "uv.h"
#include "FS.hpp"
#include "DefaultLoop.hpp"
#include "common.hpp"
#include "Argument.hpp"
  
int main(int /*argc*/,char** argv){
  FS fs;
  std::string fileName(argv[1]);
  fs.openReq_.data = &fs;
  fs.fileName(fileName).open([](uv_fs_t* req){
    FS* fs = static_cast<FS*>(req->data);
    char buffer[1024];
    fs->buffer = buffer;
    fs->read([](cpuv::Argument* arg){
	uv_fs_t req = arg->fs->readReq_;
	if(req.result<0) {
	  ERR(uv_strerror(req.result));
	  arg->fs->close(arg);
	} else if(req.result==0) arg->fs->close(arg);
	else {
	  std::cout<<arg->fs->uv_buff.base;
	  arg->fs->read();
	}
    });
  });
  DefaultLoop::getInstance().run();
}
