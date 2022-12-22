#include <iostream>
#include <cstdio>
#include <cstring>
#include "parser.h"
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

void CheckLength(const string &str, const int &len) {
    if (str.size() > len) throw error();
}

void CheckId(const string &str) {
    if (str.size() > 30) throw error();
    bool o;
    for (int i = 0, k = str.size(); i < k; ++i) {
        o = false;
        if (IsNumber(str[i]) || IsLetter(str[i]) || str[i] == '_') o = true;
        if (!o) throw error();
    }
} 

void CheckNumber(const string &str) {
    if (str.size() > 10) throw error();
    for (int i = 0, k = str.size(); i < k; ++i) {
        if (!IsNumber(str[i])) throw error();
    }
    if (stoll(str) > 2147483647) throw error();
}

void CheckControl(const string &str) {
    for (int i = 0, k = str.size(); i < k; ++i) {
        if (str[i] <= 32 && str[i] >= 127) throw error();
    }
}

void CheckFloat(const string &str) {
    if (str.size() > 13) throw error();
    if (str[0] == '.' || str[str.size()-1] == '.') throw error();
    if (str[0] == '0' && (str.size() > 1 && str[1] != '.')) throw error();
    int cnt = 0;
    for (int i = 0, k = str.size(); i < k; ++i) {
        if (!IsNumber(str[i]) && str[i] != '.') throw error();
        if (str[i] == '.') ++cnt;
    }
    if (cnt > 1) throw error();
}

void CheckKeyword(const string &str, bool o) {
    if (o && (str[0] == '|' || str[str.size()-1] == '|')) throw error();
    if (str.size() > 60) throw error();
    for (int i = 0, k = str.size(); i < k; ++i) {
        if (str[i] == '\"') throw error(); 
    }
}

int GetType(const string &str) {
    int length = str.size();
    if (length >= 6 && str.substr(0, 6) == "-ISBN=") return 4;
    if (length >= 6 && str.substr(0, 7) == "-name=\"") return 3;
    if (length >= 8 && str.substr(0, 9) == "-author=\"") return 2;
    if (length >= 9 && str.substr(0, 10) == "-keyword=\"") return 1;
    if (length >= 7 && str.substr(0, 7) == "-price=") return 0;
    throw error();
}

void Solve(const char ch[], bool &working) {
    try{
        int n = 0;
        string str[15] = {};
        int p = 0;
        while (ch[p] == ' ') ++p;
        for (int i = p, k = strlen(ch); i < k; ++i) {
            //cout << ch[i];
            if (ch[i] <= 31 || ch[i] >= 127) throw error();
            if (ch[i] == ' ') { if(i + 1 < k && ch[i+1] != ' ') ++n; continue; }
            str[n] += ch[i];
        }
        //for (int i = 0; i <= n; ++i) cout << str[i] << ' ';
        //cout << '\n';
        //cout << "n=" << n << '\n';
        if (!str[0].size()) return ;
        if (str[0] == "quit" || str[0] == "exit") {
            if (n) throw error("Invalid\n");
            working = false; 
        }
        else if (str[0] == "su") {
            CheckId(str[1]);
            if (n == 2) CheckId(str[2]), Login(str[1], str[2]);
            else if (n == 1) Login(str[1], "");
            else throw error("Invalid\n");
        }
        else if (str[0] == "logout") {
            if (n) throw error("Invalid\n");
            Logout();
        }
        else if (str[0] == "register") {
            if (n != 3) throw error("Invalid\n");
            CheckId(str[1]); CheckId(str[2]);
            CheckLength(str[3], 30);
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
            CheckLength(str[3], 1);
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
            CheckLength(str[1], 20);
            CheckNumber(str[2]);
            char ISBN[22];
            strcpy(ISBN, str[1].c_str());
            Buy(ISBN, stoi(str[2]));
        }
        else if (str[0] == "select") {
            if (n != 1) throw error();
            CheckLength(str[1], 20);
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
                    CheckLength(str[i], 20 + 6);
                    strcpy(ISBN, str[i].substr(6).c_str());
                    if (!strlen(ISBN)) throw error();
                }
                else if (o == 3) {
                    if (str[i][str[i].size()-1] != '\"') throw error();
                    CheckKeyword(str[i].substr(7, str[i].size() - 8), false);
                    strcpy(name, str[i].substr(7, str[i].size() - 8).c_str());
                    if (!strlen(name)) throw error();
                    //cout << "name=" << name << '\n';
                }
                else if (o == 2) {
                    if (str[i][str[i].size()-1] != '\"') throw error();
                    CheckKeyword(str[i].substr(9, str[i].size() - 10), false);
                    strcpy(author, str[i].substr(9, str[i].size() - 10).c_str());
                    if (!strlen(author)) throw error();
                }
                else if (o == 1) {
                    if (str[i][str[i].size()-1] != '\"') throw error();
                    CheckKeyword(str[i].substr(10, str[i].size() - 11), true);
                    strcpy(keyword, str[i].substr(10, str[i].size() - 11).c_str());
                    if (!strlen(keyword)) throw error();
                }
                else if (o == 0) {
                    if (str[i].size() <= 7) throw error();
                    CheckFloat(str[i].substr(7));
                    price = stod(str[i].substr(7));
                }
            }
            if (!type) throw error();
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
                if (n > 2) throw error();
                if (n == 1) {
                    ShowFinance(GetCount());
                }
                else {
                    CheckNumber(str[2]);
                    int count = stoi(str[2]);
                    if (!count) cout << '\n';
                    else ShowFinance(count);
                }
            }
            else {
                if (n == 0) Show(0, "");
                else {
                    if (n > 1) throw error();
                    int type = GetType(str[1]);
                    char Key[62];
                    if (type == 4) {
                        CheckLength(str[1], 20 + 6);
                        strcpy(Key, str[1].substr(6).c_str());
                    }
                    else if (type == 3) {
                        if (str[1][str[1].size()-1] != '\"') throw error();
                        CheckKeyword(str[1].substr(7, str[1].size() - 8), false);
                        strcpy(Key, str[1].substr(7, str[1].size() - 8).c_str());
                    }
                    else if (type == 2) {
                        if (str[1][str[1].size()-1] != '\"') throw error();
                        CheckKeyword(str[1].substr(9, str[1].size() - 10), false);
                        strcpy(Key, str[1].substr(9, str[1].size() - 10).c_str());
                    }
                    else if (type == 1) {
                        if (str[1][str[1].size()-1] != '\"') throw error();
                        CheckKeyword(str[1].substr(10, str[1].size() - 11), false);
                        strcpy(Key, str[1].substr(10, str[1].size() - 11).c_str());
                        for (int i = 0, k = strlen(Key); i < k; ++i) 
                            if (Key[i] == '|') throw error();
                    }
                    else throw error();
                    Show(type, Key);
                }
            }
        }
        else if (str[0] == "log") {}
        else throw error("Invalid\n");
    }
    catch (error) { cout << "Invalid\n";}
}
