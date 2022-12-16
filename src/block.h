#ifndef BOOKSTORE_BLOCK_H
#define BOOKSTORE_BLOCK_H
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using std::fstream;
using std::istream;
using std::ostream;
using std::cout;
using std::vector;

const int BlockSize = 168;

template<class T> class BlockNode;
template<class T> class BlockLinkList;

template<class T>
class mystream: public fstream{
 public:
  mystream &operator<<(const BlockNode<T> &a) {
    if(!a.place) return *this;
    seekp(a.place);
    write(reinterpret_cast<const char *>(&a), sizeof(a));
    return *this;
  }
  void getBlock(const int &o, BlockNode<T> &a) {
    seekg(o);
    read(reinterpret_cast<char *>(&a), sizeof(a));
  }
};

template<class T>
class Node{
 private:
  char Key[65];
  T Info;

 public:
  Node():Key(""), Info(T()){}
  Node(const char Key_[], const T Info_):Info(Info_){ strcpy(Key, Key_); }
  Node &operator= (const Node<T> &a) {
    if (this == &a) return *this;
    strcpy(Key, a.Key); 
    Info = a.Info;
    return *this;
  }
  char* getKey(){ return Key; }
  template<class T1>
  friend bool operator<(const Node<T1> &a, const Node<T1> &b);
  template<class T1>
  friend bool operator==(const Node<T1> &a, const Node<T1> &b); 
  template<class T1>
  friend bool operator<=(const Node<T1> &a, const Node<T1> &b); 
  template<class T1>
  friend ostream &operator<<(ostream &os, const Node<T1> &a);
  friend class BlockNode<T>;
  friend class BlockLinkList<T>;
  ~Node(){}
};

template<class T>
bool operator<(const Node<T> &a, const Node<T> &b) {
    return strcmp(a.Key, b.Key)? strcmp(a.Key, b.Key) < 0 : a.Info < b.Info;
}

template<class T>
bool operator==(const Node<T> &a, const Node<T> &b) {
    return !strcmp(a.Key, b.Key) && a.Info == b.Info;
}

template<class T>
bool operator<=(const Node<T> &a, const Node<T> &b) {
    return a < b || a == b; 
}

template<class T>
ostream &operator<< (ostream &os, const Node<T> &a) {
    os << "Key=" << a.Key << '\n' << "Info=" << a.Info << '\n';
    return os;
}

template<class T> class BlockLinkList;
template<class T>
class BlockNode {
 private:
  int prev = 0, next = 0;
  int place = 0;
  int size = 0;
  Node<T> values[BlockSize * 3 + 1] = {};  
  static const int maxSize = BlockSize << 1;
  static const int minSize = BlockSize;

 public:
  friend class BlockLinkList<T>;
  friend class mystream<T>;
  template<class T1>
  friend ostream &operator<<(ostream &os, const BlockNode<T1> &a);
  void insertNode(const Node<T> &a) {
    int p = 0;
    while (p < size && values[p] < a) ++p; //the first one who >= a
    //p = std::upper_bound(values, values + size, a) - values;
    for (int i = size - 1; i >= p; --i) values[i + 1] = values[i];
    values[p] = a;
    ++size;
  }
  void deleteNode(const int &p) {
    for (int i = p + 1; i < size; ++i) values[i - 1] = values[i];
    values[size] = Node<T>();
    --size;
  }
  bool in(const char index[]) {
    return strcmp(index, values[0].Key) >= 0 && strcmp(index, values[size - 1].Key) <= 0;
  }
  bool in(const Node<T> &a) {
    return a <= values[size - 1];
  }
  ~BlockNode(){}
};

template<class T>
ostream &operator<<(ostream &os, const BlockNode<T> &a) {
    os << a.prev << " next=" << a.next << " place=" << a.place << '\n';
    os << a.size << '\n';
    for (int i = 0; i < ((BlockSize << 1) | 1); ++i) os << a.values[i] << '\n';
    return os;
}

template<class T >
class BlockLinkList {
 private:
  mystream<T> iofile;
  char FileName[65];
  int sum = 0;
  BlockNode<T> a, b, c;
 public:
  explicit BlockLinkList(const char FileName_[]){
    strcpy(FileName, FileName_);
    iofile.open(FileName_, fstream::in);
    bool flag = iofile.is_open();
    iofile.close();
    if (!flag) {
        iofile.open(FileName_, fstream::out);
        iofile.close();
    }
    iofile.open(FileName_, fstream::in | fstream::out | fstream::binary);
    if (!flag) {
        int head = 0;
        iofile.seekp(0);
        iofile.write(reinterpret_cast<const char *>(&head), sizeof(int));
        iofile.seekg(0);
        //iofile.read(reinterpret_cast<char *>(&head), sizeof(int));        
    }
    else {
        iofile.seekg(0, mystream<T>::end);
        sum = ((int)iofile.tellg() - sizeof(int)) / sizeof(BlockNode<T>);
    }
  }
  int newBlock(BlockNode<T> &a) {
    BlockNode<T> b;
    int temp = sum * sizeof(a) + sizeof(int);
    ++sum;
    b.prev = a.place;
    b.place = temp;
    b.next = a.next;
    if (a.next) {
        iofile.getBlock(a.next, c);
        c.prev = temp;
        iofile << c;
    }
    a.next = temp;
    iofile << a << b;
    return temp;
  }
  void Split(BlockNode<T> &a) {
    int p = newBlock(a);
    iofile.getBlock(p, b);
    int sz = a.size >> 1;
    for (int i = sz; i < a.size; ++i) b.values[i - sz] = a.values[i];
    b.size = a.size - sz;
    a.size = sz;
    iofile << a << b; 
  }
  void Merge(BlockNode<T> &a) {
    if (!a.next) return ;
    iofile.getBlock(a.next, b);
    for (int i = 0; i < b.size; ++i) a.values[a.size++] = b.values[i];
    b.size = 0;
    if (b.next) {
        iofile.getBlock(b.next, c);
        c.prev = a.place;
        iofile << c;
    }
    a.next = b.next;
    if (a.size > a.maxSize) Split(a);
    else iofile << a;
  }
  void Insert(const Node<T> &value) {    
    int head = 0;
    iofile.seekg(0);
    iofile.read(reinterpret_cast<char *>(&head), sizeof(int));
    if (!head) {
      BlockNode<T> a;
      head = sizeof(int);
      iofile.seekp(0, mystream<T>::end);
      ++sum;
      a.place = head;
      iofile.seekp(0);
      iofile.write(reinterpret_cast<const char *>(&head), sizeof(int));
      a.insertNode(value);
      if (a.size > a.maxSize) Split(a);
      iofile << a;
      return ;
    }
    while(head) {
      iofile.getBlock(head, a);
      if (!a.next || a.in(value)) {
          a.insertNode(value);
          if (a.size > a.maxSize) Split(a);
          else iofile << a;
          return ;
      }
      head = a.next;
    }
  }
  void Delete(const Node<T> &value) {
    int head = 0;
    iofile.seekg(0);
    iofile.read(reinterpret_cast<char *>(&head), sizeof(int));
    if (!head) return ;
    while(head) {
      iofile.getBlock(head, a);
      if (a.size && a.in(value)) {
          int i = std::lower_bound(a.values, a.values + a.size, value) - a.values;
          if (value == a.values[i]) a.deleteNode(i);
          if (a.size < a.minSize) Merge(a);
          iofile << a;
          return ;
      }
      head = a.next;
    }
  }
  void Find(const char index[], vector<T> &array) {
    int head = 0;
    iofile.seekg(0);
    iofile.read(reinterpret_cast<char *>(&head), sizeof(int));
    bool o = false;
    while(head) {
        iofile.getBlock(head, a);
        if (a.size && a.in(index)) {
            for (int i = 0; i < a.size; ++i) {
                if (!strcmp(index, a.values[i].Key)) { o = true; array.push_back(a.values[i].Info); }
                else if(o) return ;
            }
        }
        head = a.next;
    }
    return ;
  }
  ~BlockLinkList(){
    iofile.close();
  }
};

#endif
