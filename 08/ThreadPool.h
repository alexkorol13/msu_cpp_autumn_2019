#include <thread>
#include <queue>
#include <future>
#include <vector>
#include <functional>
#include <algorithm>

class ThreadPool {
    using func = std::function<void()>;
public:
    explicit ThreadPool(size_t poolSize) {
        size = poolSize;
        living = true;
        for (size_t i = 0; i < poolSize; i++) {
            threads.emplace_back([this]() {
                while (true) {
                    if (living == false) {
                        break;
                    }
                    std::unique_lock<std::mutex> lock(semaphore);
                    if (Queue.empty()) {
                        while (true) {
                            if (living == false) {
                                break;
                            }
                            cond.wait(lock);
                            if (Queue.empty() == false) {
                                break;
                            }
                        }
                    } else {
                        auto m_tmp(std::move(Queue.front()));
                        Queue.pop();
                        lock.unlock();
                        m_tmp();
                    }
                }
            });
        }
    }

    template<class F, class... Args>
    auto exec(F f, Args... args) -> std::future<decltype(f(args...))> {
        auto promise = new std::promise<decltype(f(args...))>();
        auto future = promise->get_future();
        auto task = [this](std::promise<decltype(f(args...))> *promise, F func, Args... args) {
            maker_of_task(promise, func, args...);
        };
        func t = std::bind(task, promise, f, args...);
        semaphore.lock();
        Queue.push(t);
        semaphore.unlock();
        cond.notify_one();
        return future;
    }

    ~ThreadPool() {
        if (living == true) {
            living = false;
            cond.notify_all();
            for (size_t i = 0; i < size; i++) {
                threads[i].join();
            }
        }
    }

private:
    std::queue<func> Queue;
    std::vector<std::thread> threads;
    std::mutex semaphore;
    size_t size;
    std::condition_variable cond;
    std::atomic<bool> living;

    template<class Promise_ptr, class F, class... Args>
    void maker_of_task(Promise_ptr point, F func, Args... args) {
        point->set_value(func(args...));
        delete point;
    }

    template<class F, class... Args>
    void maker_of_task(std::promise<void> *point, F f, Args... args) {
        f(args...);
        point->set_value();
        delete point;
    }

};