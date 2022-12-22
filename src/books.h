#ifndef BOOKSTORE_BOOKS_H
#define BOOKSTORE_BOOKS_H
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stack>
#include <iomanip>
#include "users.h"
#include "block.h"
using std::stack;

class Book{
 public:
  char ISBN[22] = {}, author[62] = {}, name[62] = {};
  char keyword[62] = {};
  long long sum = 0;
  double price = 0;
  Book(){}
  Book(const Book &a):sum(a.sum), price(a.price) {
      strcpy(ISBN, a.ISBN);
      strcpy(author, a.author);
      strcpy(name, a.name);
      strcpy(keyword, a.keyword);
  }
  Book &operator=(const Book &a) {
      if (this == &a) return *this;
      sum = a.sum;
      price = a.price;
      strcpy(ISBN, a.ISBN);
      strcpy(author, a.author);
      strcpy(name, a.name);
      strcpy(keyword, a.keyword);
      return *this;
  }
  const bool operator<(const Book &b) const {
      return strcmp(ISBN, b.ISBN) < 0;
  }
  const bool operator==(const Book &b) const {
      return !strcmp(ISBN, b.ISBN);
  }
  friend ostream& operator<<(ostream &os, const Book &a);
};

ostream& operator<<(ostream &os, const Book &a);

static BlockLinkList<Book> books_ISBN("books_ISBN.db");
static BlockLinkList<Book> books_name("books_name.db");
static BlockLinkList<Book> books_author("books_author.db");
static BlockLinkList<Book> books_keyword("books_keyword.db");
static stack<Book> bookstack;

void StackPop();
void StackPush(const Book &a);
void DeleteBook(const Book &a);
void AddBook(const Book &a);
void ModifyBook(const Book &a, const Book &b);
void Show(int type, const char Key[]);
void Buy (const char Key[], const int &quantity);
void Select(const char Key[]);
void Modify(const int &type, const char ISBN[], const char name[], 
            const char author[], const char keyword[], const double &price);
void Import(const int &quantity, const double &cost);

#endif
