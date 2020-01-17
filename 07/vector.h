#pragma once

#include <iterator>
#include <exception>
#include <cstring>
#include <limits>

using namespace std;

template <class T>
class Allocator {
public:
    T *alloc(size_t size) {
        T *pointer = (T*) malloc(size * sizeof(T));
        if (pointer == nullptr) {
            throw bad_alloc();
        }
        return pointer;
    }

    template<class... Args>
    void creat(T *pointer, size_t size, const Args&... val) {
        for (size_t i = 0; i < size; i++) {
            new (pointer + i) T(val...);
        }
    }

    template<class Arg>
    void creat(T *pointer, Arg&& val) {
        new (pointer) T(forward<Arg>(val));
    }

    void destroy(T *pointer, size_t size) {
        for (size_t i = 0; i < size; i++) {
            (pointer[i]).~T();
        }
    }

    void finish(T *pointer, size_t size) {
        free(pointer);
    }

    size_t max_size() const noexcept {
        return numeric_limits<size_t>::max() / sizeof(T);
    }
};

template <class T>
class Iterator {
    T *pointer;
    bool rev_flag;
public:
    explicit Iterator(T *ptr, bool if_rev) : pointer(ptr), rev_flag(if_rev) {}

    T& operator*() const {
        return *pointer;
    }

    bool operator==(const Iterator<T>& cmp) const {
        return (pointer == cmp.pointer) && (rev_flag == cmp.rev_flag);
    }

    bool operator!=(const Iterator<T>& cmp) const {
        return !(*this == cmp);
    }

    Iterator operator+(size_t size) {
        Iterator iter(pointer, rev_flag);
        iter += size;
        return iter;
    }

    Iterator operator-(size_t size) {
        Iterator iter(pointer, rev_flag);
        iter -= size;
        return iter;
    }

    Iterator& operator++() {
        pointer = (rev_flag) ? (pointer - 1) : (pointer + 1);
        return *this;
    }

    Iterator& operator--() {
        pointer = (rev_flag) ? (pointer + 1) : (pointer - 1);
        return *this;
    }

    Iterator& operator+=(size_t size) {
        pointer = (rev_flag) ? (pointer - size) : (pointer + size);
        return *this;
    }

    Iterator& operator-=(size_t size) {
        pointer = (rev_flag) ? (pointer + size) : (pointer - size);
        return *this;
    }

    Iterator operator++(int) {
        Iterator iter(pointer, rev_flag);
        pointer = (rev_flag) ? (pointer - 1) : (pointer + 1);
        return iter;
    }

    Iterator operator--(int) {
        Iterator iter(pointer, rev_flag);
        pointer = (rev_flag) ? (pointer + 1) : (pointer - 1);
        return iter;
    }
};

template <class T>
class Vector{
    T *arr;
    Allocator<T> allocator;
    size_t cur_size;
    size_t cur_vol;
public:
    Vector()
    {
        allocator = Allocator<T>();
        arr = nullptr;
        cur_size = 0;
        cur_vol = 0;
    }

    template<class... Args>
    explicit Vector(size_t size, const Args&... val)
    {
        allocator = Allocator<T>();
        arr = allocator.alloc(size);
        allocator.creat(arr, size, val...);
        cur_size = size;
        cur_vol = size;
    }

    ~Vector()
    {
        allocator.destroy(arr, cur_size);
        allocator.finish(arr, cur_vol);
    }

    T &operator[](size_t index)
    {
        return arr[index];
    }

    const T &operator[](size_t index) const
    {
        return arr[index];
    }

    void reserve(size_t required)
    {
        if (required > cur_vol) {
            T *new_data = allocator.alloc(required);
            memcpy(new_data, arr, cur_size * sizeof(T));
            allocator.finish(arr, cur_vol);
            arr = new_data;
            cur_vol = required;
        }
    }

    bool empty()
    {
        return cur_size == 0;
    }

    void clear() noexcept
    {
        allocator.destroy(arr, cur_size);
        cur_size = 0;
    }

    template<class... Args>
    void resize(size_t new_size, const Args&... val)
    {
        if (new_size > cur_size) {
            if (new_size > cur_vol) {
                T *new_data = allocator.alloc(new_size);
                memcpy(new_data, arr, cur_size * sizeof(T));
                allocator.finish(arr, cur_vol);
                arr = new_data;
                cur_vol = new_size;
            }
            allocator.creat(arr + cur_size, new_size - cur_size, val...);
            cur_size = new_size;
        } else if (new_size < cur_size) {
            allocator.destroy(arr + new_size, cur_size - new_size);
            cur_size = new_size;
        }
    }

    size_t size()
    {
        return cur_size;
    }

    size_t capacity()
    {
        return cur_vol;
    }

    Iterator<T> begin() noexcept
    {
        return Iterator<T>(arr, false);
    }

    Iterator<T> rbegin() noexcept
    {
        return Iterator<T>(arr + cur_size - 1, true);
    }

    Iterator<T> end() noexcept
    {
        return Iterator<T>(arr + cur_size, false);
    }

    Iterator<T> rend() noexcept
    {
        return Iterator<T>(arr - 1, true);
    }

    template<class Arg>
    void push_back(Arg&& val)
    {
        if (cur_vol == 0) {
            arr = allocator.alloc(1);
            allocator.creat(arr, forward<Arg>(val));
            cur_size = 1;
            cur_vol = 1;
        } else if (cur_size < cur_vol) {
            allocator.creat(arr + cur_size, forward<Arg>(val));
            cur_size++;
        } else if (cur_size == cur_vol) {
            size_t new_vol = cur_vol * 2;
            T *new_data = allocator.alloc(new_vol);
            memcpy(new_data, arr, cur_size * sizeof(T));
            allocator.finish(arr, cur_vol);
            cur_vol = new_vol;
            arr = new_data;
            allocator.creat(arr + cur_size, forward<Arg>(val));
            cur_size++;
        }
    }

    void pop_back()
    {
        cur_size--;
        allocator.destroy(arr + cur_size, 1);
    }
};