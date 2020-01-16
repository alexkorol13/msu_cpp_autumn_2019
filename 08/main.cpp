#include <iostream>
#include <functional>

using namespace std;

class ThreadPool
{
public:
    explicit ThreadPool(size_t poolSize);

     // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>;
};

int main(void)
{
    /*
    struct A {};

    void foo(const A&) {}

    ThreadPool pool(8);

    auto task1 = pool.exec(foo, A());
    task1.get();

    auto task2 = pool.exec([]() { return 1; });
    task2.get();
    */
    cout << "Hello world!" << endl;
    return 0;
}
