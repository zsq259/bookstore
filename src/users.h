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
  string userid, username, passwd;
  int privilege;
  User(const string &id_, const string &passwd_, const string &name_, const int &privilege_):
      userid(id_), username(name_), passwd(passwd_), privilege(privilege_){}
};

static BlockLinkList<User> array = BlockLinkList<User>("users.db");
static stack<User> userstack;

void Register(const string &id, const string &passwd, const string &name) {
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (!v.empty()) throw error("Invalid\n");
    User a = User(id, passwd, name, 1);
    array.Insert(Node(userid, a));
}

void Passwd(const string &id, const string &curpasswd, const string &passwd) {
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (v.empty()) throw error("Invalid\n");
    User a = v.back();
    if (userstack.empty()) throw error("Invalid\n");
    User now = userstack.top();
    if (curpasswd == "") {
        if (now.privilege < 7) throw error("Invalid\n");
    }
    else if (curpasswd != a.passwd) throw error("Invalid\n");
    array.Delete(Node(userid, a));
    a.passwd = passwd;
    array.Insert(Node(userid, a));
}

void Useradd(const string &id, const string &passwd, const int &privilege, const string &name) {
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (!v.empty()) throw error("Invalid\n");
    if (userstack.empty()) throw error("Invalid\n");
    if (userstack.top().privilege <= privilege) throw error("Invalid\n");
    User a = User(id, passwd, name, privilege);
    array.Insert(Node(userid, a));
}

void Delete(const string &id) {
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (v.empty()) throw error("Invalid\n");
    User now = userstack.top();
    if (now.userid == id) throw error("Invalid\n");
    User a = v.back();
    array.Delete(Node(userid, a));
}

void Login(const string &id, const string &passwd) {
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (v.empty()) throw error("Invalid\n");
    User a = v.back();
    if (passwd == "") {
        if (userstack.empty()) throw error("Invalid\n");
        if (userstack.top().privilege <= a.privilege) throw error("Invalid\n");
    }
    if (passwd != a.passwd) throw error("Invalid\n");
    userstack.push(a);
}

void Logout() {
    if (userstack.empty()) throw error("Invalid\n");
    userstack.pop();
}

#endif
