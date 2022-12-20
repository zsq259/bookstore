#include <iostream>
#include <cstdio>
#include <cstring>
#include "users.h"
#include "books.h"
#include "diary.h"
#include "block.h"

bool IsNumber(char c) {
    return c >= '0' && c <= '9';
}

bool IsLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void CheckId(const string &str) {
    bool o;
    for (int i = 0, k = str.size(); i < k; ++i) {
        o = false;
        if (IsNumber(str[i]) || IsLetter(str[i]) || str[i] == '_') o = true;
        if (!o) throw error();
    }
} 

void CheckNumber(const string &str) {
    for (int i = 0, k = str.size(); i < k; ++i) {
        if (!IsNumber(str[i])) throw error();
    }
}

void CheckFloat(const string &str) {
    for (int i = 0, k = str.size(); i < k; ++i) {
        if (!IsNumber(str[i]) && str[i] != '.') throw error();
    }
}

void CheckKeyword(const string &str) {
    for (int i = 0, k = str.size(); i < k; ++i) {
        if (str[i] == '\"') throw error(); 
    }
}

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
            if (n == 2) CheckId(str[1]), CheckId(str[2]), Login(str[1], str[2]);
            else if (n == 1) CheckId(str[1]), Login(str[1], "");
            else throw error("Invalid\n");
        }
        else if (str[0] == "logout") {
            if (n) throw error("Invalid\n");
            Logout();
        }
        else if (str[0] == "register") {
            if (n != 3) throw error("Invalid\n");
            CheckId(str[1]); CheckId(str[2]);
            Register(str[1], str[2], str[3]);
        }
        else if (str[0] == "passwd") {
            CheckId(str[1]); CheckId(str[2]);
            if (n == 3) CheckId(str[3]);
            if (n == 3) Passwd(str[1], str[2], str[3]);
            else if (n == 2) Passwd(str[1], "", str[2]);
            else throw error("Invalid\n");
        }
        else if (str[0] == "useradd") {
            if (n != 4) throw error("Invalid\n");
            CheckId(str[1]); CheckId(str[2]);
            CheckNumber(str[3]);
            Useradd(str[1], str[2], stoi(str[3]), str[4]);
        }
        else if (str[0] == "delete") {
            if (n != 1) throw error("Invalid\n");
            CheckId(str[1]);
            Delete(str[1]);
        }
        else if (str[0] == "buy") {
            if (n != 2) throw error();
            CheckKeyword(str[1]);
            CheckNumber(str[2]);
            char ISBN[22];
            strcpy(ISBN, str[1].c_str());
            Buy(ISBN, stoi(str[2]));
        }
        else if (str[0] == "select") {
            if (n != 1) throw error();
            CheckKeyword(str[1]);
            char ISBN[22];
            strcpy(ISBN, str[1].c_str());
            Select(ISBN);
        }
        else if (str[0] == "modify") {

        }
        else if (str[0] == "import") {
            if (n != 2) throw error();
            CheckNumber(str[1]);
            CheckFloat(str[2]);
            Import(stoi(str[1]), stod(str[2]));
        }
        else if (str[0] == "show") {
            if (str[1] == "finance") {}
            else {}
        }
        else if (str[0] == "log") {}
        else throw error("Invalid\n");
    }
    catch (error) { cout << "Invalid\n";}
}
