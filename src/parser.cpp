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

int GetType(const string &str) {
    int length = str.size();
    if (length >= 6 && str.substr(0, 6) == "-ISBN=") return 4;
    if (length >= 6 && str.substr(0, 6) == "-name=") return 3;
    if (length >= 8 && str.substr(0, 8) == "-author=") return 2;
    if (length >= 9 && str.substr(0, 9) == "-keyword=") return 1;
    if (length >= 7 && str.substr(0, 7) == "-price=") return 0;
    throw error();
}

void Solve(const char ch[], bool &working) {
    int n = 0;
    string str[15] = {};
    for (int i = 0, k = strlen(ch); i < k; ++i) {
        if (ch[i] == ' ') { if(i && ch[i-1] != ' ') ++n; continue; }
        str[n] += ch[i];
    }
    //cout << ch << '\n';
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
            int type = 0;
            char ISBN[22], name[62], author[62], keyword[62];
            double price = 0;
            for (int i = 1; i <= n; ++i) {
                int o = GetType(str[i]);
                if (type & (1 << o)) throw error();
                type |= (1 << o);
                if (o == 4) {
                    strcpy(ISBN, str[i].substr(6).c_str());
                }
                else if (o == 3) {
                    CheckKeyword(str[i].substr(7, str[i].size() - 8));
                    strcpy(name, str[i].substr(7, str[i].size() - 8).c_str());
                    //cout << "name=" << name << '\n';
                }
                else if (o == 2) {
                    CheckKeyword(str[i].substr(9, str[i].size() - 10));
                    strcpy(author, str[i].substr(9, str[i].size() - 10).c_str());
                }
                else if (o == 1) {
                    CheckKeyword(str[i].substr(10, str[i].size() - 11));
                    strcpy(keyword, str[i].substr(10, str[i].size() - 11).c_str());
                }
                else if (o == 0) {
                    CheckFloat(str[i].substr(7));
                    price = stod(str[i].substr(7));
                }
            }
            Modify(type, ISBN, name, author, keyword, price);
        }
        else if (str[0] == "import") {
            if (n != 2) throw error();
            CheckNumber(str[1]);
            CheckFloat(str[2]);
            Import(stoi(str[1]), stod(str[2]));
        }
        else if (str[0] == "show") {
            if (str[1] == "finance") {
                
            }
            else {
                if (n == 0) Show(0, "");
                else {
                    if (n > 1) throw error();
                    int type = GetType(str[1]);
                    char Key[62];
                    if (type == 4) {
                        strcpy(Key, str[1].substr(6).c_str());
                    }
                    else if (type == 3) {
                        CheckKeyword(str[1].substr(7, str[1].size() - 8));
                        strcpy(Key, str[1].substr(7, str[1].size() - 8).c_str());
                    }
                    else if (type == 2) {
                        CheckKeyword(str[1].substr(9, str[1].size() - 10));
                        strcpy(Key, str[1].substr(9, str[1].size() - 10).c_str());
                    }
                    else if (type == 1) {
                        CheckKeyword(str[1].substr(10, str[1].size() - 11));
                        strcpy(Key, str[1].substr(10, str[1].size() - 11).c_str());
                        for (int i = 0, k = strlen(Key); i < k; ++i) 
                            if (Key[i] == '|') throw error();
                    }
                    Show(type, Key);
                }
            }
        }
        else if (str[0] == "log") {}
        else throw error("Invalid\n");
    }
    catch (error) { cout << "Invalid\n";}
}