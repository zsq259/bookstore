#include <iostream>
#include <cstdio>
#include <cstring>
using std::string;

class error{
 private:
  string msg;

 public:
  error(const string &msg_):msg(msg_){} 
};