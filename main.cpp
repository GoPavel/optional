#include <iostream>
#include "optional.h"

template class optional<int>;

int main () {
    std::cout << "OPTIONAL" << std::endl;

    {
        optional<int> op;
    }
    {
        int a = 5;
        optional<int> op = a;
//        assert((*op) == 5);
    }

    return 0;
}
