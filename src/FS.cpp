#include <iostream>
#include "FS.hpp"
#include "DefaultLoop.hpp"
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
FS& FS::open(cpuv_cb cb){
  cpuv::Argument* arg = new cpuv::Argument;
  variantMap* argMap = new variantMap;
  (*argMap)["cb"] = cb; (*argMap)["arg"] = arg;
  arg->fs = this;
  openReq_.data = static_cast<void*>(argMap);
  uv_fs_open(DefaultLoop::getInstance().getDefaultLoop(),
      &openReq_,fileName().data(),flags(),mode(),[](uv_fs_t* uv_req){
	static variantMap* argMap =  static_cast<variantMap*>(uv_req->data);
	static cpuv::Argument* arg = boost::get<cpuv::Argument*>((*argMap)["arg"]);
	static cpuv_cb cb = static_cast<cpuv_cb>(boost::get<cpuv_cb>((*argMap)["cb"]));
	CLEAN_ON_FIRST_INVOCATION(argMap);
	arg->status = (arg->fs->openReq_.result< 0)?Status::ERROR:Status::ALL_GOOD;
	if(arg->status == Status::ERROR)
	  arg->errorMsg = uv_strerror(arg->fs->openReq_.result);
	cb(arg);
      });
  return *this;
}
FS& FS::read(cpuv_cb cb,int64_t offset){
  if(cb) {
    cpuv::Argument* arg = new cpuv::Argument;
    arg->fs = this;
    variantMap* argMap = new variantMap;
    (*argMap)["cb"] = cb; (*argMap)["arg"] = arg;
    readReq_.data = static_cast<void*>(argMap);
    onRead = [](uv_fs_t* uv_req){
      static variantMap* argMap =  static_cast<variantMap*>(uv_req->data);
      static cpuv::Argument* arg = boost::get<cpuv::Argument*>((*argMap)["arg"]);
      static cpuv_cb cb = static_cast<cpuv_cb>(boost::get<cpuv_cb>((*argMap)["cb"]));
      CLEAN_ON_FIRST_INVOCATION(argMap)
      arg->status = (arg->fs->readReq_.result< 0)?Status::ERROR:
                    (arg->fs->readReq_.result==0)?Status::END_OF_FILE:Status::ALL_GOOD;
      if(arg->status == Status::ERROR)
	arg->errorMsg = uv_strerror(arg->fs->readReq_.result);
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
  REF_CLEANUP(openReq_); REF_CLEANUP(readReq_);
}
