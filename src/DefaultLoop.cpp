#include "DefaultLoop.hpp"
#include "uv.h"

uv_loop_t* DefaultLoop::getDefaultLoop() const{
  return uv_default_loop();
}
void DefaultLoop::run() const{
  uv_run(getDefaultLoop(),UV_RUN_DEFAULT);
}
