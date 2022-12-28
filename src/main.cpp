#include <iostream>
#include <cstdio>
#include <cstring>
#include "users.h"
#include "books.h"
#include "diary.h"
#include "block.h"
#include "parser.h"
using std::cin;
using std::string;

int main() {
    string str;
    char ch[645];
    bool working = true;
    Init(); // creat a super uesr root
    InitLog();
    while(working) {
        if (!getline(cin, str)) working = false;
        else {
            if (str.size() > 640) cout << "Invalid\n";
            else { strcpy(ch, str.c_str()); Solve(ch, working);} //process the command 
        }
    }
    AddLog("", "system exited!");
    WriteLog();
}