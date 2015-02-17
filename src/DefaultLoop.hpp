#include <iostream>
#include "uv.h"
#pragma once
namespace cpuv{
  class DefaultLoop {
    private:
      DefaultLoop(){};
    public:
      DefaultLoop(DefaultLoop const&)     = delete;
      void operator=(DefaultLoop const&)  = delete;
      static DefaultLoop& getInstance(){
	static DefaultLoop self;
	return self; 
      }
      uv_loop_t* getDefaultLoop() const;
      void run() const;
  };
}
