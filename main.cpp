#include <iostream>
#include "optional.h"
#include <type_traits>


template class optional_movable<int>;
template class optional_copyable<int>;

class A {
public:
    int a;
    A() : a(5) { }
};

class Copy {
public:
    int a;
    Copy(): a(5) { }
    Copy( Copy const & other): a(other.a) {}
};

class Move {
public:
    int a;
    Move(): a(5) { }
    Move( Move && other): a(std::move(other.a)) {}
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
    {
        assert(std::is_copy_constructible<optional<Copy>>::value == true );
        assert(std::is_copy_constructible<optional<Move>>::value == false);
    }

    return 0;
}
