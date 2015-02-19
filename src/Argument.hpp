#include <iostream>
#include <map>
#include <boost/variant.hpp>
#include "common.hpp"
#include "Buffer.hpp"
#include "Status.hpp"

#pragma once
namespace cpuv{
  template <class Caller> class Argument{
      void* capture_{nullptr};
      Buffer buffer_{nullptr};
      Caller* this_{nullptr};
    public:
      std::pair<std::string, std::string> fileName;
      Argument(){};
      Argument(void* capture,Caller* caller):capture_(capture),this_(caller){};
      Status status{Status::ALL_GOOD};
      const char* errorMsg{nullptr};
      template<class Capture>
      Capture& getCapture(){ return *static_cast<Capture*>(capture_); }
      template<class Capture>
      Argument& setCapture(Capture* capture){
	capture_= capture; return *this;
      }
      Buffer& buffer(){ return buffer_; }
      Caller& _this(){ return *this_; }
      Argument& _this(Caller& this_){ this->this_=&this_; return *this; }
  };
}
