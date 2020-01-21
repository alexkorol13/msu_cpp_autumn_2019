#pragma once

#include <iterator>
#include <exception>
#include <cstring>
#include <limits>


template <class T>
class Allocator {
public:
    T *alloc(std::size_t size) {
        T *pointer = (T*) malloc(size * sizeof(T));
        if (pointer == nullptr) {
            throw std::bad_alloc();
        }
        return pointer;
    }

    template<class... Args>
    void construct(T *pointer, std::size_t size, const Args&... val) {
        for (std::size_t i = 0; i < size; i++) {
            new (pointer + i) T(val...);
        }
    }

    template<class Arg>
    void construct(T *pointer, Arg&& val) {
        new (pointer) T(std::forward<Arg>(val)); ///3
    }

    void destroy(T *pointer, std::size_t size) {
        for (std::size_t i = 0; i < size; i++) {
            (pointer[i]).~T();
        }
    }

    void finish(T *pointer, std::size_t size) {
        free(pointer);
    }

    std::size_t max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(T);
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

    Iterator operator+(std::size_t size) {
        Iterator iter(pointer, rev_flag);
        iter += size;
        return iter;
    }

    Iterator operator-(std::size_t size) {
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

    Iterator& operator+=(std::size_t size) {
        pointer = (rev_flag) ? (pointer - size) : (pointer + size);
        return *this;
    }

    Iterator& operator-=(std::size_t size) {
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
    std::size_t cur_size;
    std::size_t cur_vol;
public:
    Vector()
    {
        allocator = Allocator<T>();
        arr = nullptr;
        cur_size = 0;
        cur_vol = 0;
    }


    template<class... Args>
    explicit Vector(std::size_t size, const Args&... val)
    {
        allocator = Allocator<T>();
        arr = allocator.alloc(size);
        allocator.construct(arr, size, val...);
        cur_size = size;
        cur_vol = size;
    }


    ~Vector()
    {
        allocator.destroy(arr, cur_size);
        allocator.finish(arr, cur_vol);
    }


    T &operator[](std::size_t index)
    {
        return arr[index];
    }


    const T &operator[](std::size_t index) const
    {
        return arr[index];
    }


    void reserve(std::size_t new_vol)
    {
        if (new_vol > cur_vol)
        {
            T *new_arr = allocator.alloc(new_vol);
            for (std::size_t i = 0; i < cur_size; i++)
            {
                allocator.construct(new_arr + i, std::move(arr[i]));
            }
            allocator.destroy(arr, cur_size);
            allocator.finish(arr, cur_vol); /// ??
            cur_vol = new_vol;
            arr = new_arr; /// ??
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
    void resize(std::size_t new_size, const Args&... val)
    {
        if (new_size < cur_size)
        {
            allocator.destroy(arr + new_size, cur_size - new_size);
            cur_size = new_size;
        }

        else if (new_size > cur_size)
        {
            if (new_size > cur_vol)
            {
                T *new_arr = allocator.alloc(new_size);
                for (size_t i = 0; i < cur_size; i++)
                {
                    allocator.construct(new_arr + i, std::move(arr[i]));
                }
                allocator.destroy(arr, cur_size);
                allocator.finish(arr, cur_vol);
                arr = new_arr;
                cur_size = new_size;
            }
            allocator.construct(arr + cur_size, new_size - cur_size, val...);
            cur_size = new_size;
        }
    }


    std::size_t size()
    {
        return cur_size;
    }


    std::size_t capacity()
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
    void push_back(Arg&& value) {
        if (cur_vol == 0) {
            arr = allocator.alloc(1);
            allocator.construct(arr, std::move(value));
            cur_size = 1;
            cur_vol = 1;
        }
        else if (cur_size == cur_vol) {
            T *new_arr = allocator.alloc(2 * cur_vol);
            for (std::size_t i = 0; i < cur_size; i++) {
                allocator.construct(new_arr + i, std::move(arr[i])); ///1
            }
            allocator.destroy(arr, cur_size);
            allocator.finish(arr, cur_vol);
            arr = new_arr;
            allocator.construct(arr + cur_size++, std::move(value));
            cur_vol *= 2;
        }
        else if (cur_size < cur_vol) {
            allocator.construct(arr + cur_size++, std::move(value));
        }
    }


    void pop_back()
    {
        cur_size--;
        allocator.destroy(arr + cur_size, 1);
    }
};