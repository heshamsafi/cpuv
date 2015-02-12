#include <iostream>
#include <functional>
#include "Argument.hpp"
#include "uv.h"
#include "common.hpp"
#include <map>
#include <boost/variant.hpp>
#pragma once
typedef std::map<std::string,boost::variant<cpuv::Argument*,cpuv_cb>> variantMap;
class FS{
  std::string fileName_;
  int flags_;
  int mode_;
  public:
  FS():flags_(O_RDONLY),mode_(0){
    readReq_.ptr = openReq_.ptr = nullptr;
  };
  ~FS();
  uv_fs_cb onRead;
  uv_fs_t openReq_;
  char* buffer;
  uv_buf_t uv_buff;
  uv_fs_t readReq_;
  uv_fs_t closeReq_;
  std::string name;
  std::string fileName() const;
  FS& fileName(std::string&);
  int flags() const;
  FS& flags(int);
  int mode() const;
  FS& mode(int);
  FS& open(cpuv_cb);
  FS& read(cpuv_cb=nullptr,int64_t=-1);
  FS& close(cpuv::Argument* =nullptr,uv_fs_cb=nullptr);
};
