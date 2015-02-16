#include <iostream>
#include <algorithm>
#include <common.hpp>

#pragma once
namespace cpuv{
  class Buffer{
    public:
    char* buffer_;
    Buffer(void*):buffer_(nullptr) {};
    Buffer(size_t buffSize=sizeof(int)+1):buffer_(new char[buffSize]){ }; 
    //Buffer(const Buffer& that){
      //buffer_ = new char[strlen(that.buffer_)];
      //memcpy(buffer_,that.buffer_,strlen(that.buffer_)+1);
    //};
    Buffer(Buffer&& that){  
      buffer_ = that.buffer_; 
      that.buffer_ = nullptr;
    };
    Buffer& operator=(Buffer& that){
      std::swap(buffer_,that.buffer_); return *this;
    }
    Buffer& operator=(Buffer&& that){
      this->buffer_ = that.buffer_;
      that.buffer_ = nullptr;
      return *this;
    }
    ~Buffer(){
      if(buffer_) 
	delete[] buffer_; 
    }
    Buffer& setNullTerminator(size_t buffSize){
      buffer_[buffSize] = '\0';
      return *this;
    }
    template <typename T>
    T get(){ return static_cast<T>(buffer_); }
    size_t size(){ return sizeof(buffer_); }
  };
}
