#ifndef BOOKSTORE_DIARY_H
#define BOOKSTORE_DIARY_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include "block.h"
using std::string;

static BlockLinkList<double> logs("logs.db");
static int logscnt;

void InitLog();
void WriteLog();
void NewDeal(const double &sum);
void ShowFinance(const int &count);
int GetCount();

#endif
