#include <iostream>
#include "FS.hpp"
#include "DefaultLoop.hpp"
#include <functional>
#include <common.hpp>

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
FS& FS::read(uv_fs_cb cb,int64_t offset){
  if(cb) onRead = cb; 
  if(openReq_.result >= 0) {
    uv_buff =uv_buf_init(buffer,sizeof(buffer));
    uv_fs_read(DefaultLoop::getInstance().getDefaultLoop()
	, &readReq_, openReq_.result,
	&uv_buff, /*nbr of buffs*/1, offset, onRead);
  }else ERR( uv_strerror((int)openReq_.result) ); 
  return *this;
}
FS& FS::close(uv_fs_cb cb) {
  uv_fs_close(DefaultLoop::getInstance().getDefaultLoop(),
      &closeReq_,openReq_.result,cb);
  return *this;
}
FS::~FS(){
  uv_fs_req_cleanup(&openReq_);
  uv_fs_req_cleanup(&readReq_);
}
