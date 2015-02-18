#include <iostream>
#include "FS.hpp"
#include "DefaultLoop.hpp"
#include <common.hpp>
#include "Argument.hpp"
#include <map>
#include <boost/variant.hpp>

namespace cpuv{
  std::string FS::fileName() const{ return fileName_; }
  FS& FS::fileName(std::string& fileName_){
    this->fileName_= fileName_; return *this;
  }
  FS& FS::fileName(std::string&& fileName_){
    this->fileName_= fileName_; return *this;
  }
  int FS::flags() const{ return flags_; }
  FS& FS::flags(int flags_){
    this->flags_ = flags_; return *this;
  }
  int FS::mode() const{ return mode_; } 
  FS& FS::mode(int mode_){ this->mode_ = mode_; return *this; }
  int FS::open(cpuv_cb<FS> cb,void* capture){
    uv_fs_cb uv_cb = nullptr;
    openReq_ = new uv_fs_t;
    if(cb) {
      Argument<FS>* arg=new Argument<FS>(capture);
      openReq_->data = packArgMap(cb,arg);
      uv_cb = 
	LIBUV_CB( 
	  uv_fs_t& openReq_ = *arg->_this().openReq_;
	  arg->status = (openReq_.result< 0)?Status::ERROR:Status::ALL_GOOD;
	  if(arg->status == Status::ERROR)
	    arg->errorMsg = uv_strerror(openReq_.result);
	  cb(*arg); delete arg; delete argMap;
	)
    }
    return uv_fs_open(DefaultLoop::getInstance().getDefaultLoop(),
	openReq_,fileName().data(),flags(),mode(),uv_cb);
  }
  int FS::readSync(int fd,Argument<FS>& arg,int64_t offset){
    uv_buf_t uv_buff = uv_buf_init(arg.buffer().get<char*>(),arg.buffer().capacity);
    readReq_ = new uv_fs_t;
    int ret = uv_fs_read(DefaultLoop::getInstance().getDefaultLoop()
	, readReq_, fd,
	&uv_buff, /*nbr of buffs*/1, offset, nullptr);
    uv_fs_req_cleanup(readReq_);
    delete readReq_;
    return ret;
  }
  int FS::read(cpuv_cb<FS> cb,void* capture ,int64_t offset){
    static uv_buf_t uv_buff;
    if(cb) {
      readReq_ = new uv_fs_t;
      if(onRead){
	variantMap* argMap 
	  = static_cast<variantMap*>(readReq_->data),&argMapAlias = *argMap;
	Argument<FS>* arg 
	  = boost::get<Argument<FS>*>(argMapAlias["arg"]);
	delete arg; delete argMap;
      }
      Argument<FS>* arg= new Argument<FS>(capture);
      arg->buffer().resize(5);
      readReq_->data = packArgMap(cb,arg);
      uv_buff = uv_buf_init(arg->buffer().get<char*>(),arg->buffer().capacity);
      onRead = LIBUV_CB(
      auto& caller = arg->_this();
       	uv_fs_t& readReq_ = *caller.readReq_;
	arg->status = (readReq_.result< 0)?Status::ERROR:
	  (readReq_.result==0)?Status::END_OF_FILE:Status::ALL_GOOD;
	if(arg->status == Status::ERROR)
	  arg->errorMsg = uv_strerror(readReq_.result);
	  arg->buffer().size = readReq_.result;
	if(arg->status == Status::ALL_GOOD)
	  arg->buffer().setNullTerminator(readReq_.result);
	  //std::cout<<"("<<readReq_.result<<")"<<std::endl;
	cb(*arg);
	if(arg->status == Status::END_OF_FILE) {
	  uv_fs_req_cleanup(arg->_this().readReq_);
	  delete arg->_this().readReq_;
	  delete argMap; delete arg;
	}
      )
    }
    int ret = uv_fs_read(DefaultLoop::getInstance().getDefaultLoop()
	, readReq_, openReq_->result,
	&uv_buff, /*nbr of buffs*/1, offset, onRead);
    if(!cb && !onRead){
      uv_fs_req_cleanup(readReq_);
      delete readReq_;
    }
    return ret;
  }
  int FS::write(char* str,size_t size,cpuv_cb<FS> cb,void* capture,int fd,int64_t offset){
    uv_fs_cb uv_cb = nullptr;
    writeReq_ = new uv_fs_t;
    Argument<FS>* arg=new Argument<FS>(capture);
    writeReq_->data = packArgMap(cb,arg);
    uv_cb = LIBUV_CB(
      uv_fs_t& writeReq_ = *arg->_this().writeReq_;
      arg->status = (writeReq_.result< 0)?Status::ERROR:Status::ALL_GOOD;
      if(arg->status == Status::ERROR)
	arg->errorMsg = uv_strerror(writeReq_.result);
      cb(*arg);
      uv_fs_req_cleanup(arg->_this().writeReq_);
      delete arg->_this().writeReq_;
      delete argMap; delete arg;
    )
    const uv_buf_t uv_buff = uv_buf_init(str,size); 
    int ret = uv_fs_write(DefaultLoop::getInstance().getDefaultLoop(), writeReq_,fd>-1?fd:openReq_->result,
	&uv_buff, 1, offset, uv_cb);
    return ret;
  }
  int FS::writeSync(int fd,char* str,size_t size,int64_t offset){
    writeReq_ = new uv_fs_t;
    uv_buf_t uv_buff = uv_buf_init(str,size);
    int ret = uv_fs_write(DefaultLoop::getInstance().getDefaultLoop()
	, writeReq_, fd,
	&uv_buff, /*nbr of buffs*/1, offset, nullptr);
    uv_fs_req_cleanup(writeReq_);
    delete writeReq_;
    return ret;
  }
  int FS::close(cpuv_cb<FS> cb,void* capture) {
    closeReq_ = new uv_fs_t;
    uv_fs_cb uv_cb = nullptr;
    if(cb) {
      Argument<FS>* arg=new Argument<FS>(capture);
      closeReq_->data = packArgMap(cb,arg);
      uv_cb = LIBUV_CB(
	cb(*arg);
	uv_fs_req_cleanup(arg->_this().openReq_);
	uv_fs_req_cleanup(arg->_this().closeReq_);
	delete arg->_this().closeReq_;
	delete arg->_this().openReq_; delete argMap; delete arg; 
      )
    }
    int ret = uv_fs_close(DefaultLoop::getInstance().getDefaultLoop(),
	closeReq_,openReq_->result,uv_cb);
    if(!cb){
      uv_fs_req_cleanup(openReq_);
      uv_fs_req_cleanup(closeReq_);
      delete closeReq_;
      delete openReq_;
    } 
    return ret;
  }
  int FS::unlink(std::string& fileName,cpuv_cb<FS> cb,void* capture){
    unlinkReq_ = new uv_fs_t;
    uv_fs_cb uv_cb = nullptr;
    if(cb) {
      Argument<FS>* arg=new Argument<FS>(capture);
      unlinkReq_->data = packArgMap(cb,arg);
      uv_cb = LIBUV_CB(
	arg->status = (arg->_this().unlinkReq_>0)?Status::ALL_GOOD:Status::ERROR;
	cb(*arg);
	uv_fs_req_cleanup(arg->_this().unlinkReq_);
	delete arg->_this().unlinkReq_;
	delete argMap; delete arg; 
	)
    }
    int ret = uv_fs_unlink(DefaultLoop::getInstance().getDefaultLoop(),
	unlinkReq_,fileName.data(),uv_cb);
    if(!cb){
      uv_fs_req_cleanup(unlinkReq_);
      delete unlinkReq_;
    } 
    return ret;
  }
  void* FS::packArgMap(cpuv_cb<FS> cb,Argument<FS>* arg){
    variantMap* argMap = new variantMap, &argMapAlias=*argMap;
    argMapAlias["cb"] = cb; argMapAlias["arg"] = &arg->_this(*this);
    return argMap;
  }
}
