bin_PROGRAMS=bin/main bin/test-fs
LIBR=src/DefaultLoop.cpp src/FS.cpp
bin_main_SOURCES=src/main.cpp $(LIBR)
bin_test_fs_SOURCES=test/test-fs.cpp $(LIBR)
TESTS=bin/test-fs
#LDADD=deps/libuv/.libs/libuv.a -lrt -lpthread -lnsl -ldl
LDADD=deps/libuv/.libs/libuv.so 
bin_test_fs_LDADD=/usr/lib/libboost_unit_test_framework.a $(LDADD)
AM_CPPFLAGS=-g3 -O0 -std=c++11 -Wall -Ideps/libuv/include -Isrc -pipe
CXX=clang++
