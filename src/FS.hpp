#include <iostream>
#include "Argument.hpp"
#include "uv.h"
#include "common.hpp"
#include <Buffer.hpp>
#pragma once

#define LIBUV_CB(code)                                                                      \
  [](uv_fs_t* uv_req){                                                                     \
    variantMap* argMap = static_cast<variantMap*>(uv_req->data), argMapAlias = *argMap;    \
    Argument<FS>* arg = boost::get<Argument<FS>*>(argMapAlias["arg"]);                     \
    cpuv_cb<FS> cb = boost::get<cpuv_cb<FS> >(argMapAlias["cb"]);                          \
    code                                                                                   \
  };                                                                                       \

namespace cpuv{
  class FS{
    uv_fs_cb onRead{nullptr};
    uv_fs_t* openReq_;
    uv_fs_t* readReq_;
    uv_fs_t* writeReq_;
    uv_fs_t* closeReq_;
    uv_fs_t* unlinkReq_;
    std::string fileName_;
    int flags_;
    int mode_;
    void* packArgMap(cpuv_cb<FS>,Argument<FS>*);
    public:
    FS():flags_(O_RDONLY),mode_(0){ };
    std::string fileName() const;
    FS& fileName(std::string&);
    FS& fileName(std::string&&);
    Buffer& buffer();
    int flags() const;
    FS& flags(int);
    int mode() const;
    FS& mode(int);
    int open(cpuv_cb<FS> cb=nullptr,void* capture=nullptr);
    int read(cpuv_cb<FS> cb=nullptr,void* capture=nullptr,int64_t offset=-1);
    int write(char* str,size_t size,cpuv_cb<FS> cb,void* capture=nullptr,int fd=-1,int64_t offset=-1);
    int readSync(int fd,Argument<FS>&,int64_t offset=-1);
    int writeSync(int fd,char* str,size_t size,int64_t offset=-1);
    int close(cpuv_cb<FS> =nullptr,void* capture=nullptr);
    int unlink(std::string& str,cpuv_cb<FS> cb=nullptr,void* capture=nullptr);
  };
  using variantMap = std::map<std::string,boost::variant<Argument<FS>*,cpuv_cb<FS>>>;
};
