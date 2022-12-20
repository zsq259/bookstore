#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stack>
#include <iomanip>
#include "users.h"
#include "block.h"
#include "books.h"
#include "diary.h"
using std::cout;
using std::fixed;
using std::setprecision;
using std::vector;

ostream& operator<<(ostream &os, const Book &a) {
    os << a.ISBN << '\t' << a.name << '\t' << a.author << '\t';
    if (a.keycnt) {
        os << a.keyword[0];
        for (int i = 1; i < a.keycnt; ++i) os << '|' << a.keyword[i];
    }
    os << '\t';
    os << fixed << setprecision(2) << a.price << '\t' << a.sum << '\n';
    return os;
}

void StackPop() {
    bookstack.pop();
}

void StackPush(const Book &a) {
    bookstack.push(a);
}

void DeleteBook(const Book &a) {
    books_ISBN.Delete(Node<Book>(a.ISBN, a));
    books_name.Delete(Node<Book>(a.name, a));
    books_author.Delete(Node<Book>(a.author, a));
    for (int i = 0; i < a.keycnt; ++i) books_keyword.Delete(Node<Book>(a.keyword[i], a));
}

void AddBook(const Book &a) {
    books_ISBN.Insert(Node<Book>(a.ISBN, a));
    books_name.Insert(Node<Book>(a.name, a));
    books_author.Insert(Node<Book>(a.author, a));
    for (int i = 0; i < a.keycnt; ++i) books_keyword.Insert(Node<Book>(a.keyword[i], a));
}

void ModifyBook(const Book &a, const Book &b) {
    DeleteBook(a);
    DeleteBook(b);
    AddBook(b);
}

void Show(int type, const char Key[]) {
    if (GetPrivilege() < 1) throw error();
    if (type && !strlen(Key)) throw error();
    vector<Book> v;
    if (type == 0) books_ISBN.FindAll(v);
    else if (type == 4) books_ISBN.Find(Key, v);
    else if (type == 3) books_name.Find(Key, v);
    else if (type == 2) books_author.Find(Key, v);
    else if (type == 1) books_keyword.Find(Key, v);
    if (v.empty()) cout << '\n';
    else for (Book x:v) cout << x;
}

void Buy (const char Key[], const int &quantity) {
    if (GetPrivilege() < 1) throw error();
    vector<Book> v;
    books_ISBN.Find(Key, v);
    if (v.empty() || quantity <= 0) throw error();
    Book a = v.front();
    if (quantity > a.sum) throw error();
    a.sum -= quantity;
    cout << fixed << setprecision(2) << a.price * quantity << '\n';
    NewDeal(a.price * quantity);
    ModifyBook(a, a);
} 

void Select(const char Key[]) {
    if (GetPrivilege() < 3) throw error();
    if (bookstack.empty()) throw error();
    vector<Book> v;
    Book a;
    books_ISBN.Find(Key, v);
    if (v.empty()) {
        strcpy(a.ISBN, Key);
        AddBook(a);    
    }
    else a = v.front();
    bookstack.pop();
    bookstack.push(a);
}

void Modify(const int &type, const char ISBN[], const char name[], 
            const char author[], const char keyword[], const double &price) {
    if (GetPrivilege() < 3) throw error();
    if (bookstack.empty()) throw error();
    vector<Book> v;
    Book a = bookstack.top();
    books_ISBN.Find(a.ISBN, v);
    if (!v.empty()) a = v.back();
    Book b = a;
    if (!strlen(a.ISBN)) throw error(); 
    if (type & (1 << 4)) {
        if (!strcmp(a.ISBN, ISBN)) throw error();
        books_ISBN.Find(ISBN, v);
        if (!v.empty()) throw error();
        strcpy(a.ISBN, ISBN);
    }
    if (type & (1 << 3)) strcpy(a.name, name);
    if (type & (1 << 2)) strcpy(a.author, author);
    if (type & (1 << 1)) {
        for (int i = 0; i < a.keycnt; ++i) {
            strcpy(a.keyword[i], "");
        }
        a.keycnt = 0;
        for (int i = 0, k = strlen(keyword); i < k; ++i) {
            if (keyword[i] == '|') { a.keycnt++; continue; }
            strncat(a.keyword[a.keycnt], keyword + i, 1);
        }
        a.keycnt++;
        for (int i = 0; i < a.keycnt; ++i) 
            for (int j = i + 1; j < a.keycnt; ++j) 
                if (!strcmp(a.keyword[i], a.keyword[j])) throw error();
    }
    if (type & 1) a.price = price;
    bookstack.pop();
    bookstack.push(a);
    stack<Book> temp;
    while (!bookstack.empty()) {
        Book now = bookstack.top();
        if (now == b) now = a;
        temp.push(now);
        bookstack.pop();
    }
    while (!temp.empty()) {
        bookstack.push(temp.top());
        temp.pop();
    }
    ModifyBook(b, a);
}

void Import(const int &quantity, const double &cost) {
    if (GetPrivilege() < 3) throw error();
    if (bookstack.empty() || !strlen(bookstack.top().ISBN)) throw error();
    if (quantity <= 0 || cost <= 0) throw error();
    Book a = bookstack.top();
    a.sum += quantity;
    NewDeal(-cost);
    ModifyBook(a, a);
}