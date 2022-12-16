#include <iostream>
#include <cstdio>
#include <cstring>
#include "users.h"
#include "books.h"
#include "diary.h"
#include "block.h"
using std::cin;
using std::string;

int main() {
    char ch[641];
    bool working = true;
    Init();
    while(working) {
        int n = 0;
        string str[15] = {};
        cin.getline(ch, 640);
        for (int i = 0, k = strlen(ch); i < k; ++i) {
            if (ch[i] == ' ') { if(i && ch[i-1] != ' ') ++n; continue; }
            str[n] += ch[i];
        }
        try {
            if (str[0] == "quit" || str[0] == "exit") { working = false; }
            else if (str[0] == "su") {
                if (n == 2) Login(str[1], str[2]);
                else if (n == 1) Login(str[1], "");
            }
            else if (str[0] == "logout") {
                Logout();
            }
            else if (str[0] == "registr") {
                Register(str[1], str[2], str[3]);
            }
            else if (str[0] == "passwd") {
                if (n == 3) Passwd(str[1], str[2], str[3]);
                else if (n == 2) Passwd(str[1], "", str[2]);
            }
            else if (str[0] == "useradd") {
                Useradd(str[1], str[2], str[3][0] - '0', str[4]);
            }
            else if (str[0] == "delete") {
                Delete(str[1]);
            }
        }
        catch (error) { cout << "Invalid\n";}
    }
}