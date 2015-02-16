bin_PROGRAMS=bin/main
bin_main_SOURCES=src/main.cpp src/DefaultLoop.cpp src/FS.cpp
LDADD = deps/libuv/.libs/libuv.so $(INTLLIBS)
AM_CPPFLAGS = -g -O0 -std=c++11 -Wall -Ideps/libuv/include -Isrc -pipe 
CXX=clang++
