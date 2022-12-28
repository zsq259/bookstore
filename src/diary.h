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

void InitLog(); // get how many previous counts
void WriteLog(); // write how many counts until system close
void NewDeal(const double &sum); // add a deal
void ShowFinance(const int &count);
int GetCount(); // how many counts

void Log();
void AddLog(const string user, const char ch[]);
#endif
