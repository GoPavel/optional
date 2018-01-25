#include <iostream>
#include "optional.h"

template class optional<int>;

class A {
public:
    int a;
    A() : a(5) { }
};

int main () {
    std::cout << "OPTIONAL" << std::endl;

    {
        optional<int> op;
    }
    {
        int a = 5;
        optional<int> op = a;
        assert((*op) == 5);
    }
    {
        A a;
        optional<A> op = a;
        assert(op->a == 5);
    }

    return 0;
}
