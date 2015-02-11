#include <iostream>
#include "uv.h"
#include "FS.hpp"
#include "DefaultLoop.hpp"
#include "common.hpp"
  
int main(int /*argc*/,char** argv){
  FS fs;
  std::string fileName(argv[1]);
  fs.readReq_.data = fs.openReq_.data = &fs;
  fs.fileName(fileName).open([](uv_fs_t* req){
    FS* fs = static_cast<FS*>(req->data);
    char buffer[1024];
    fs->buffer = buffer;
    fs->read([](uv_fs_t* req){
	//FS* fs = static_cast<FS*>(req->data);
	//if(req->result<0) ERR(uv_strerror(req->result));
	//else if(req->result == 0) fs->close();
	//else {
	//  std::cout<<fs->uv_buff.base;
	//  fs->read();
	//}
    });
  });
  DefaultLoop::getInstance().run();
}
