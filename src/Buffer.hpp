#include <iostream>
#include <common.hpp>

#pragma once
namespace cpuv{
  class Buffer{
    char* buffer_{nullptr};
    public:
    size_t capacity{0};
    size_t size{0};
    Buffer(void*):buffer_(nullptr) {};
    Buffer(size_t buffSize):buffer_(new char[buffSize+1]),capacity(buffSize){ }; 
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
    Buffer& resize(size_t newSize){
      if(buffer_) delete[] buffer_;
      buffer_ = new char[newSize+1];
      capacity = newSize;
      return *this;
    }
    ~Buffer(){
      if(buffer_) 
	delete[] buffer_; 
    }
    Buffer& setNullTerminator(size_t buffSize){
      buffer_[buffSize] = '\0';
      size=buffSize; return *this;
    }
    template <typename T>
    T get(){ return static_cast<T>(buffer_); }
  };
}
