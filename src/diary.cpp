#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include "diary.h"
#include "block.h"
#include "exception.h"
#include "users.h"
using std::cout;
using std::fixed;
using std::setprecision;
using std::string;
using std::to_string;

void InitLog() { // get how many previous counts
    vector<double> v;
    finances.Find(to_string(0).c_str(), v);
    if (!v.empty()) financescnt = v.back();
}

void WriteLog() { // write how many counts until system close 
    finances.Insert(Node<double>(to_string(0).c_str(), financescnt));
}

void NewDeal(const double &sum) { // add a deal
    ++financescnt;
    finances.Insert(Node<double>(to_string(financescnt).c_str(), sum));
}

void ShowFinance(const int &count) {
    if (GetPrivilege() < 7) throw error();
    if (count < 0 || count > financescnt) throw error();
    double in = 0, out = 0;
    vector<double> v;
    for (int i = 0; i < count; ++i) {
        finances.Find(to_string(financescnt - i).c_str(), v);
        double ret = v.back();
        if (ret > 0) in += ret;
        else out -= ret;
    }
    cout << fixed << setprecision(2) << "+ " << in << " - " << out << '\n';
}

int GetCount() { return financescnt; } // how many counts

void Log() {
    if (GetPrivilege() < 7) throw error();
    cout << "-------log-------\n";
    logs.open("logs.db", fstream::in);
    if (!logs) {
        puts("ojbk");
        logs.close();
        logs.open("logs.db", fstream::out);
        logs.close();
        logs.open("logs.db", fstream::in);
    }
    logs.seekg(fstream::beg);
    char ch[645];
    while(logs.getline(ch, 640)) {
        cout << ch << '\n';
    }
    cout << "-----finances----\n";
    vector<double> v;
    for (int i = 0; i < financescnt; ++i) {
        finances.Find(to_string(i).c_str(), v);
        double ret = v.back();
        if (ret >= 0) cout << fixed << setprecision(2) << "+ " << ret << '\n';
        else cout << fixed << setprecision(2) << "- " << -ret << '\n';
    }
    logs.close();
    cout << "-----------------\n";
}

void AddLog(const string user, const char ch[]) {
    logs.open("logs.db", fstream::app);
    if (!logs) {
        logs.close();
        logs.open("logs.db", fstream::out);
    }
    if (user.size()) logs << user << ": " << ch << '\n';
    else logs << ch << '\n';
    logs.close();
}