#include <iostream>
#include <cstdio>
#include <cstring>
#include "users.h"
#include "books.h"
#include "diary.h"
#include "block.h"

void Solve(const char ch[], bool &working) {
    int n = 0;
    string str[15] = {};
    for (int i = 0, k = strlen(ch); i < k; ++i) {
        if (ch[i] == ' ') { if(i && ch[i-1] != ' ') ++n; continue; }
        str[n] += ch[i];
    }
    try {
        if (str[0] == "quit" || str[0] == "exit") {
            if (n) throw error("Invalid\n");
            working = false; 
        }
        else if (str[0] == "su") {
            if (n == 2) Login(str[1], str[2]);
            else if (n == 1) Login(str[1], "");
            else throw error("Invalid\n");
        }
        else if (str[0] == "logout") {
            Logout();
        }
        else if (str[0] == "register") {
            if (n != 3) throw error("Invalid\n");
            Register(str[1], str[2], str[3]);
        }
        else if (str[0] == "passwd") {
            if (n == 3) Passwd(str[1], str[2], str[3]);
            else if (n == 2) Passwd(str[1], "", str[2]);
            else throw error("Invalid\n");
        }
        else if (str[0] == "useradd") {
            if (n != 4) throw error("Invalid\n");
            Useradd(str[1], str[2], str[3][0] - '0', str[4]);
        }
        else if (str[0] == "delete") {
            if (n != 1) throw error("Invalid\n");
            Delete(str[1]);
        }
        else if (str[0] == "buy") {}
        else if (str[0] == "select") {}
        else if (str[0] == "modify") {}
        else if (str[0] == "import") {}
        else if (str[0] == "import") {}
        else if (str[0] == "show") {
            if (str[1] == "finance") {}
            else {}
        }
        else if (str[0] == "log") {}
        else throw error("Invalid\n");
    }
    catch (error) { cout << "Invalid\n";}
}
