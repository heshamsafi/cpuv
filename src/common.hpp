#include <functional>
#pragma once

namespace cpuv{
template <class Caller> class Argument;
  template <class Caller>
  using cpuv_cb //= void (*)(Argument<Caller>&);
                  = std::function<void(Argument<Caller>&)>;
}

#define ERR(msg) std::cerr<<msg<<std::endl 
#define CLEAN_ON_FIRST_INVOCATION(ptr) {	\
  static bool visited = false;			\
  if(!visited){ delete ptr; visited = true; }	\
}

#define REF_CLEANUP(req) if(req.ptr) uv_fs_req_cleanup(&req);
#define FN_APPLY(type,fn, ...){			             \
  void* stopper_for_apply = (int[]){0};				     \
  type** list_for_apply = (type*[]){__VA_ARGS__,stopper_for_apply};  \
  for(int i=0;list_for_apply[i]!=stopper_for_apply;++i)              \
    fn(list_for_apply[i]);                                           \
}
#define REF_CLEANUP_BATCH(...) FN_APPLY(uv_fs_t, REF_CLEANUP, __VA_ARGS__);
