#include <iostream>
#include <cassert>
#include "ThreadPool.h"

struct A {};

void foo(const A&) {}

int main() {

    ThreadPool pool(8);

    auto task1 = pool.exec(foo, A());
    task1.get();

    auto task2 = pool.exec([]() { return 1; });
    assert(task2.get() == 1);

    std::cout << "Done" << std::endl;

    return 0;
}