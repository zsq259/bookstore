#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include "block.h"
#include "users.h"
#include <vector>
using namespace std;

int main(){
    char a1[64] = "helgo";
    char b1[64] = "www";
    strncat(b1, a1 + 1, 1);
    cout << b1 << '\n';
    
}