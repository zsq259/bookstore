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
    char ch[641];
    bool working = true;
    Init();
    while(working) {
        if (!cin.getline(ch, 640)) working = false;
        else Solve(ch, working);
    }
}