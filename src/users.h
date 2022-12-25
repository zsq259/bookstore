#ifndef BOOKSTORE_USERS_H
#define BOOKSTORE_USERS_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include <stack>
#include <vector>
#include "block.h"
#include "exception.h"
using std::string;
using std::stack;
using std::vector;

class User {
 public:
  char userid[64], username[64], passwd[64];
  int privilege;
  User(){}
  User(const string &id_, const string &passwd_, const string &name_, const int &privilege_):
      privilege(privilege_){
          strcpy(userid, id_.c_str());
          strcpy(username, name_.c_str());
          strcpy(passwd, passwd_.c_str());
      }
  User(const User &a) {
      strcpy(userid, a.userid);
      strcpy(username, a.username);
      strcpy(passwd, a.passwd);
      privilege = a.privilege;
  }
  User &operator=(const User &a) {
      if (this == &a) return *this;
      strcpy(userid, a.userid);
      strcpy(username, a.username);
      strcpy(passwd, a.passwd);
      privilege = a.privilege;
      return *this;
  }
  const bool operator<(const User &b) const { return strcmp(userid, b.userid) < 0; }
  const bool operator==(const User &b) const { return !strcmp(userid, b.userid); }
};

static BlockLinkList<User> array("users.db");
static stack<User> userstack;

void Init();
void Register(const string &id, const string &passwd, const string &name);
void Passwd(const string &id, const string &curpasswd, const string &passwd);
void Useradd(const string &id, const string &passwd, const int &privilege, const string &name);
void Delete(const string &id);
void Login(const string &id, const string &passwd);
void Logout();
int GetPrivilege();
string NowUser();

#endif
