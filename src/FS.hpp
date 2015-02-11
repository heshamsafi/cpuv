#include <iostream>
#include <functional>
#include "uv.h"
#pragma once
class FS{
  std::string fileName_;
  int flags_;
  int mode_;
  public:
  FS():flags_(O_RDONLY),mode_(0){};
  ~FS();
  uv_fs_cb onRead;
  uv_fs_t openReq_;
  char* buffer;
  uv_buf_t uv_buff;
  uv_fs_t readReq_;
  uv_fs_t closeReq_;
  std::string fileName() const;
  FS& fileName(std::string&);
  int flags() const;
  FS& flags(int);
  int mode() const;
  FS& mode(int);
  FS& open(uv_fs_cb);
  FS& read(uv_fs_cb=nullptr,int64_t=-1);
  FS& close(uv_fs_cb=nullptr);
};
