#ifndef BOOKSTORE_EXCEPTION_H
#define BOOKSTORE_EXCEPTION_H
#include <iostream>
#include <cstdio>
#include <cstring>
using std::string;

class error{
 private:
  string msg;

 public:
  error(){}
  error(const string &msg_):msg(msg_){} 
};

#endif