#include <iostream>
#include "Argument.hpp"
#include "uv.h"
#include "common.hpp"
#include <Buffer.hpp>
#pragma once

#define CPUV_CB(code)                                                                      \
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
    int open(cpuv_cb<FS> =nullptr,void* =nullptr);
    int read(cpuv_cb<FS> =nullptr,void* =nullptr,int64_t=-1);
    int write(char*,size_t,cpuv_cb<FS>,void* =nullptr,int=-1,int64_t=-1);
    int readSync(int,Argument<FS>&,int64_t =-1);
    int writeSync(int,char*,size_t,int64_t =-1);
    int close(cpuv_cb<FS> =nullptr,void* =nullptr);
  };
  using variantMap = std::map<std::string,boost::variant<Argument<FS>*,cpuv_cb<FS>>>;
};
