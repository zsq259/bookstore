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
    while(working) {
        int n = 0;
        string str[15] = {};
        cin.getline(ch, 640);
        for (int i = 0, k = strlen(ch); i < k; ++i) {
            if (ch[i] == ' ') { if(i && ch[i-1] != ' ') ++n; continue; }
            str[n] += ch[i];
        }
        if (str[0] == "quit" || str[0] == "exit") { working = false; }
        else if (str[0] == "su") {
            
        }
    }
}