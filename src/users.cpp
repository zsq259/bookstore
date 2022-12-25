#include <cstring>
#include "users.h"
#include "books.h"

void Init() {
    char userid[32] = "root";
    User a(userid, "sjtu", "root", 7);
    vector<User> v;
    array.Find(userid, v); 
    if (!v.empty()) return ;
    array.Insert(Node<User>(userid, a));
}

void Register(const string &id, const string &passwd, const string &name) {
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (!v.empty()) throw error();
    User a(id, passwd, name, 1);
    array.Insert(Node<User>(userid, a));
}

void Passwd(const string &id, const string &curpasswd, const string &passwd) {
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (v.empty()) throw error();
    User a = v.back();
    if (userstack.empty()) throw error();
    User now = userstack.top();
    if (curpasswd == "") {
        if (now.privilege < 7) throw error();
    }
    else if (strcmp(curpasswd.c_str(), a.passwd)) throw error();
    array.Delete(Node<User>(userid, a));
    strcpy(a.passwd, passwd.c_str());
    array.Insert(Node<User>(userid, a));
}

void Useradd(const string &id, const string &passwd, const int &privilege, const string &name) {
    if (privilege != 1 && privilege != 3) throw error();
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (!v.empty()) throw error();
    if (userstack.empty()) throw error();
    if (userstack.top().privilege < 3 || userstack.top().privilege <= privilege) throw error();
    User a(id, passwd, name, privilege);
    array.Insert(Node<User>(userid, a));
}

void Delete(const string &id) {
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (v.empty() || userstack.empty()) throw error();
    User now = userstack.top();
    if (now.privilege < 7) throw error();
    bool flag = true;
    stack<User> temp;
    while (!userstack.empty()) {
        if (!strcmp(userstack.top().userid, id.c_str())) flag = false;
        temp.push(userstack.top());
        userstack.pop();
    }
    while (!temp.empty()) {
        userstack.push(temp.top());
        temp.pop();
    }
    if (!flag) throw error();
    User a = v.back();
    array.Delete(Node<User>(userid, a));
}

void Login(const string &id, const string &passwd) {
    char userid[32];
    strcpy(userid, id.c_str());
    vector<User> v;
    array.Find(userid, v); 
    if (v.empty()) throw error();
    User a = v.back();
    if (passwd == "") {
        if (userstack.empty()) throw error();
        if (userstack.top().privilege <= a.privilege) throw error();
    }
    else if (strcmp(passwd.c_str(), a.passwd)) throw error();
    userstack.push(a);
    StackPush(Book());
}

void Logout() {
    if (userstack.empty()) throw error();
    StackPop();
    userstack.pop();
}

int GetPrivilege() {
    if (userstack.empty()) return 0;
    return userstack.top().privilege;
}

string NowUser() {
    if (userstack.empty()) return "guest";
    return userstack.top().username; 
}