#include <iostream>
#include "except.h"

void run() {
    THROW_TRACE("some exception descript");
}

void foo() {
    try {
        run();
    } catch(const std::exception& e) {
        RETHROW_TRACE("exception pass through foo");
    }
}

void bar() {
    try {
        foo();
    } catch(...) {
        //std::throw_with_nested(cbl::loc_except(LOC_STRING, "bar catch&throw"));
        RETHROW_TRACE("exception pass through bar");
    }
}

void wak() {
    try {
        bar();
    } catch(...) {
        RETHROW_TRACE("exception pass through wak");
    }
}



int main() {
    try {
        wak();
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
        //print_exception(e);
        return 1;
    }
    return 0;
}
