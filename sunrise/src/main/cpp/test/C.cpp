#include "C.h"
#include "B.h"


jclass C::bArr[10] = {nullptr};

void C::test() {
    B b = B(1);
    doSomeThing(b);
}

void C::doSomeThing(A &a) {
    a.func1();
}
