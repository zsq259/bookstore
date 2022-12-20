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

void InitLog() {
    vector<double> v;
    logs.Find(to_string(0).c_str(), v);
    if (!v.empty()) logscnt = v.back();
}

void WriteLog() {
    logs.Insert(Node<double>(to_string(0).c_str(), logscnt));
}

void NewDeal(const double &sum) {
    ++logscnt;
    logs.Insert(Node<double>(to_string(logscnt).c_str(), sum));
}

void ShowFinance(const int &count) {
    if (GetPrivilege() < 7) throw error();
    if (count < 0 || count > logscnt) throw error();
    double in = 0, out = 0;
    vector<double> v;
    for (int i = 0; i < count; ++i) {
        logs.Find(to_string(logscnt - i).c_str(), v);
        double ret = v.back();
        if (ret > 0) in += ret;
        else out -= ret;
    }
    cout << fixed << setprecision(2) << "+ " << in << " - " << out << '\n';
}

int GetCount() { return logscnt; }
