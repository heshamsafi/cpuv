#define BOOST_TEST_MODULE FILE_SYSTEM
#include "boost/test/unit_test.hpp"
#include <iostream>
#include "FS.hpp"
#include "DefaultLoop.hpp"
#include "common.hpp"
#include "Argument.hpp"
#include "cstring"
#include "sstream"

using namespace cpuv;
std::string fn{"test/test.txt"};
char* str = 
"1234567890";
FS fs; 

BOOST_AUTO_TEST_CASE(WRITE_SYNC){ 
  int fd = fs.flags(O_CREAT|O_WRONLY).mode(0644).fileName(fn).open(); 
  int bytes = fs.writeSync(fd, str, strlen(str));
  fs.close();
  BOOST_CHECK_EQUAL(bytes, strlen(str));
  DefaultLoop::getInstance().run();
}

BOOST_AUTO_TEST_CASE(READ_SYNC){
  int fd = fs.flags(O_RDONLY).fileName(fn).open();
  Argument<FS> arg;
  arg.buffer().resize(10);
  int bytes = fs.readSync(fd, arg);
  BOOST_CHECK_EQUAL(bytes, strlen(str));
  BOOST_CHECK_EQUAL(strcmp(arg.buffer().get<const char*>(), str), 0);
  fs.close();
  DefaultLoop::getInstance().run();
}

BOOST_AUTO_TEST_CASE(UNLINK_SYNC){
 fs.unlink(fn);
 DefaultLoop::getInstance().run();
}

BOOST_AUTO_TEST_CASE(WRITE_ASYNC){ 
  fs.flags(O_CREAT|O_WRONLY).mode(0644).fileName(fn).open([](Argument<FS>& arg){
    if(Status::ERROR==arg.status) throw "ASYNC FILE OPEN FAILED";
    arg._this().write(str, strlen(str), [](Argument<FS>& arg){
      if(Status::ERROR==arg.status) throw "ASYNC FILE WRITE FAILED";
      fs.close();
    });
  }); 
  DefaultLoop::getInstance().run();
}

BOOST_AUTO_TEST_CASE(READ_ASYNC){
  fs.flags(O_RDONLY).fileName(fn).open([](Argument<FS>& arg){
    if(Status::ERROR==arg.status) throw "ASYNC FILE OPEN FAILED";
    arg._this().read([](Argument<FS>& arg){
      if(Status::ERROR==arg.status) throw "ASYNC FILE READ FAILED";
      static std::stringstream stream;
      if(arg.status == Status::END_OF_FILE){
        BOOST_CHECK_EQUAL( strcmp(stream.str().data(),str), 0);
        arg._this().close();
      }else{
	stream << arg.buffer().get<const char*>();
        arg._this().read();
      }
    });
  });
  DefaultLoop::getInstance().run();
}

BOOST_AUTO_TEST_CASE(UNLINK_ASYNC){
 fs.unlink(fn,[](Argument<FS>& arg){
  if(Status::ERROR==arg.status) throw "ASYNC UNLINK READ FAILED";
 });
 DefaultLoop::getInstance().run();
}
