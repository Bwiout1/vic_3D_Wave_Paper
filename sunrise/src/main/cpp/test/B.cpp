#include "B.h"
#include <iostream>

using namespace std;

void B::func1() {
    cout<<"func1"<<endl;
}

void B::func2() {
    cout<<"func2"<<endl;
}

B::B(int _num) {
    num = _num;
}
