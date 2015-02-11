#include <iostream>
#include "FS.hpp"
#include "DefaultLoop.hpp"
#include <functional>
#include <common.hpp>
#include "Argument.hpp"
#include <map>
#include <boost/variant.hpp>

std::string FS::fileName() const{ return fileName_; }
FS& FS::fileName(std::string& fileName_){
  this->fileName_= fileName_;
  return *this;
}
int FS::flags() const{ return flags_; }
FS& FS::flags(int flags_){
  this->flags_ = flags_;
  return *this;
}
int FS::mode() const{ return mode_; } 
FS& FS::mode(int mode_){
  this->mode_ = mode_;
  return *this;
}
FS& FS::open(uv_fs_cb cb){
  uv_fs_open(DefaultLoop::getInstance().getDefaultLoop(),
      &openReq_,fileName().data(),flags(),mode(),cb);
  return *this;
}
FS& FS::read(cpuv_cb cb,int64_t offset){
  if(cb) {
    cpuv::Argument* arg = new cpuv::Argument;
    arg->fs = this;
    typedef std::map<std::string,boost::variant<cpuv::Argument*,cpuv_cb>> variantMap;
    variantMap* argMap = new variantMap;
    (*argMap)["cb"] = cb;
    (*argMap)["arg"] = arg;
    readReq_.data = static_cast<void*>(argMap);
    onRead = [](uv_fs_t* uv_req){
      static variantMap* argMap =  static_cast<variantMap*>(uv_req->data);
      static cpuv::Argument* arg = boost::get<cpuv::Argument*>((*argMap)["arg"]);
      static cpuv_cb cb = static_cast<cpuv_cb>(boost::get<cpuv_cb>((*argMap)["cb"]));
      static bool visited = false;
      if(!visited){ delete argMap; visited = true; }
      cb(arg);
    }; 
  }
  if(openReq_.result >= 0) {
    uv_buff =uv_buf_init(buffer,sizeof(buffer));
    uv_fs_read(DefaultLoop::getInstance().getDefaultLoop()
	, &readReq_, openReq_.result,
	&uv_buff, /*nbr of buffs*/1, offset, onRead);
  }else ERR( uv_strerror((int)openReq_.result) ); 
  return *this;
}
FS& FS::close(cpuv::Argument* arg,uv_fs_cb cb) {
  if(arg) delete arg;
  uv_fs_close(DefaultLoop::getInstance().getDefaultLoop(),
      &closeReq_,openReq_.result,cb);
  return *this;
}
FS::~FS(){
  uv_fs_req_cleanup(&openReq_);
  uv_fs_req_cleanup(&readReq_);
}
