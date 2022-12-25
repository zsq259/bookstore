#ifndef BOOKSTORE_DIARY_H
#define BOOKSTORE_DIARY_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include "block.h"
using std::string;
using std::fstream;

static BlockLinkList<double> finances("finances.db");
static int financescnt;
static fstream logs;

void InitLog();
void WriteLog();
void NewDeal(const double &sum);
void ShowFinance(const int &count);
int GetCount();

void Log();
void AddLog(const string user, const char ch[]);
#endif
