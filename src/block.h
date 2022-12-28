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

const int BlockSize = 328;

template<class T> class BlockNode;
template<class T> class BlockLinkList;

template<class T>
class mystream: public fstream{
 public:
  mystream &operator<<(const BlockNode<T> &a) {
    if(!a.Info.place) return *this;
    seekp(a.Info.place);
    write(reinterpret_cast<const char *>(&a), sizeof(a));
    return *this;
  }
  void getBlock(const int &o, BlockNode<T> &a) {
    seekg(o);
    read(reinterpret_cast<char *>(&a), sizeof(a));
  }
  void getPart(const int &o, BlockNode<T> &a) {
    seekg(o);
    read(reinterpret_cast<char *>(&a.Info), sizeof(a.Info));
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

template<class T>
class BlockInfo {
 public:
  int prev = 0, next = 0;
  int place = 0;
  int size = 0;
  Node<T> mx, mn;
};

template<class T> class BlockLinkList;
template<class T>
class BlockNode {
 private:
  BlockInfo<T> Info;
  Node<T> values[BlockSize * 3 + 1] = {};  
  static const int maxSize = BlockSize << 1;
  static const int minSize = BlockSize;

 public:
  friend class BlockLinkList<T>;
  friend class mystream<T>;
  template<class T1>
  friend ostream &operator<<(ostream &os, const BlockNode<T1> &a);
  void update() {
    Info.mn = values[0];
    Info.mx = values[Info.size - 1];
  }
  void insertNode(const Node<T> &a) {
    int p = 0;
    while (p < Info.size && values[p] < a) ++p; //the first one who >= a
    for (int i = Info.size - 1; i >= p; --i) values[i + 1] = values[i];
    values[p] = a;
    ++Info.size;
    update();
  }
  void deleteNode(const int &p) {
    for (int i = p + 1; i < Info.size; ++i) values[i - 1] = values[i];
    values[Info.size] = Node<T>();
    --Info.size;
    update();
  }
  bool in(const char index[]) {
    return strcmp(index, Info.mn.Key) >= 0 && strcmp(index, Info.mx.Key) <= 0;
  }
  bool in(const Node<T> &a) {
    return a <= Info.mx;
  }
  ~BlockNode(){}
};

template<class T>
ostream &operator<<(ostream &os, const BlockNode<T> &a) {
    os << a.Info.prev << " next=" << a.Info.next << " place=" << a.Info.place << '\n';
    os << a.Info.size << '\n';
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
    b.Info.prev = a.Info.place;
    b.Info.place = temp;
    b.Info.next = a.Info.next;
    if (a.Info.next) {
        iofile.getBlock(a.Info.next, c);
        c.Info.prev = temp;
        iofile << c;
    }
    a.Info.next = temp;
    a.update();
    iofile << a << b;
    return temp;
  }
  void Split(BlockNode<T> &a) {
    int p = newBlock(a);
    iofile.getBlock(p, b);
    int sz = a.Info.size >> 1;
    for (int i = sz; i < a.Info.size; ++i) b.values[i - sz] = a.values[i];
    b.Info.size = a.Info.size - sz;
    a.Info.size = sz;
    a.update();
    b.update();
    iofile << a << b; 
  }
  void Merge(BlockNode<T> &a) {
    if (!a.Info.next) return ;
    iofile.getBlock(a.Info.next, b);
    for (int i = 0; i < b.Info.size; ++i) a.values[a.Info.size++] = b.values[i];
    b.Info.size = 0;
    if (b.Info.next) {
        iofile.getBlock(b.Info.next, c);
        c.Info.prev = a.Info.place;
        iofile << c;
    }
    a.Info.next = b.Info.next;
    a.update();
    if (a.Info.size > a.maxSize) Split(a);
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
      a.Info.place = head;
      iofile.seekp(0);
      iofile.write(reinterpret_cast<const char *>(&head), sizeof(int));
      a.insertNode(value);
      if (a.Info.size > a.maxSize) Split(a);
      iofile << a;
      return ;
    }
    while(head) {
      iofile.getPart(head, a);
      if (!a.Info.next || a.in(value)) {
          iofile.getBlock(head, a);
          a.insertNode(value);
          if (a.Info.size > a.maxSize) Split(a);
          else iofile << a;
          return ;
      }
      head = a.Info.next;
    }
  }
  void Delete(const Node<T> &value) {
    int head = 0;
    iofile.seekg(0);
    iofile.read(reinterpret_cast<char *>(&head), sizeof(int));
    if (!head) return ;
    while(head) {
      iofile.getPart(head, a);
      if (a.Info.size && a.in(value)) {
          iofile.getBlock(head, a);
          int i = std::lower_bound(a.values, a.values + a.Info.size, value) - a.values;
          if (value == a.values[i]) a.deleteNode(i);
          if (a.Info.size < a.minSize) Merge(a);
          iofile << a;
          return ;
      }
      head = a.Info.next;
    }
  }
  void Find(const char index[], vector<T> &array) {
    array.clear();
    int head = 0;
    iofile.seekg(0);
    iofile.read(reinterpret_cast<char *>(&head), sizeof(int));
    bool o = false;
    while(head) {
        iofile.getBlock(head, a);
        if (a.Info.size && a.in(index)) {
            for (int i = 0; i < a.Info.size; ++i) {
                if (!strcmp(index, a.values[i].Key)) { o = true; array.push_back(a.values[i].Info); }
                else if(o) return ;
            }
        }
        head = a.Info.next;
    }
    return ;
  }
  void FindAll(vector<T> &array) {
    array.clear();
    int head = 0;
    iofile.seekg(0);
    iofile.read(reinterpret_cast<char *>(&head), sizeof(int));
    while(head) {
        iofile.getBlock(head, a);
        for (int i = 0; i < a.Info.size; ++i) {
            array.push_back(a.values[i].Info);
        }
        head = a.Info.next;
    }
    return ;
  }
  ~BlockLinkList(){
    iofile.close();
  }
};

#endif
