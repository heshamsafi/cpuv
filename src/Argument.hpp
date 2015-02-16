#include "uv.h"
#include <iostream>
#include "Status.hpp"
#include <map>
#include <boost/variant.hpp>
#include "common.hpp"
#pragma once
namespace cpuv{
  class FS;
  template <class Caller> class Argument{
      Caller* caller_;
      void* capture_;
    public:
      Argument(void* capture):capture_(capture),status(Status::ALL_GOOD),errorMsg(nullptr){};
      Argument<Caller>& setCaller(Caller* caller){ caller_ = caller; return *this; }
      Caller& getCaller(){ return *caller_;  }
      Status status;
      const char* errorMsg;
      template<class Capture>
      Capture& getCapture(){ return *static_cast<Capture*>(capture_); }
      template<class Capture>
      Argument& setCapture(Capture* capture){
	capture_= capture; return *this;
      }
  };
}
