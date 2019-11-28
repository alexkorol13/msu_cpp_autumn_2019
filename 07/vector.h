#include <stdint.h>
#include <iterator>
#include <iostream>
#include <cstring>

#pragma once

template<class T>
class Allocator
{
    void constructor(T* ptr) {
        new (ptr) T();
    }

    void constructor(T* ptr, const T& x) {
        new (ptr) T(x);
    }

    void constructor(T* ptr, T&& x) {
        new (ptr) T(std::move(x));
    }

    T* alloc(size_t count) {
        T* ptr = static_cast<T*>(malloc(count* sizeof(count)));
        return ptr;
    }

    void destroy (T* ptr) {
        ptr->~T();
    }

    void free_(T* ptr, size_t count) {
        free(ptr);
    }

    T* ptr;
};


template <typename T>
class Iterator : public std::iterator<std::forward_iterator_tag, T>
{
public:
    explicit Iterator(T* ptr) : ptr(ptr) {};

    bool operator==(const Iterator<T>& iter)
    {
        return ptr == iter;
    }

    bool operator!=(const Iterator<T>& iter)
    {
        return ptr != iter;
    }

    T operator+(const T& arg)
    {
        return ptr + arg;
    }

    T operator-(const T& arg)
    {
        return ptr - arg;
    }

    T operator++(const T& arg)
    {
        ptr++;
        return *this;
    }

    T operator--(const T& arg)
    {
        ptr--;
        return *this;
    }

    T* operator*()
    {
        return *ptr;
    }

private:
    T* ptr;
};


template<typename T>
class reversed_iterator
{
    public:
        reversed_iterator(T* ptr) :
            ptr(ptr)
        { }

        T& operator*()
        {
            return *ptr;
        }

        const T& operator*() const
        {
            return *ptr;
        }

        reversed_iterator& operator++()
        {
            ptr--;
            return *this;
        }

        reversed_iterator& operator--()
        {
            ptr++;
            return *this;
        }

        reversed_iterator operator++(int)
        {
            reversed_iterator iter(ptr);
            ptr--;
            return iter;
        }

        reversed_iterator operator--(int)
        {
            reversed_iterator iter(ptr);
            ptr++;
            return iter;
        }

        reversed_iterator operator+(size_t add) const
        {
            return reversed_iterator(ptr - add);
        }

        reversed_iterator operator-(size_t m) const
        {
            return reversed_iterator(ptr + m);
        }

        bool operator==(const reversed_iterator<T> &other)
        {
            return ptr == other.ptr;
        }

        bool operator!=(const reversed_iterator<T> &other) const
        {
            return ptr != other.ptr;
        }
    private:
        T* ptr;
};


template <class T, class Alloc = Allocator<T>>
class Vector
{
private:
	T* buffer;
	size_t size_;
	size_t capacity_;
    Alloc alloc_;

public:
	Vector(size_t capacity = 10) :
		capacity_(capacity), size_(0)
	{
		buffer = alloc_.alloc(capacity_);
	}

	~Vector()
	{
		for(int i = 0; i < size_; i++) {
			alloc_.destroy(buffer+i);
		}
		alloc_.free_(buffer);
		size_ = 0;
		capacity_ = 0;
	}

	Iterator<T> begin()
	{
		return Iterator<T>(buffer);
	}

	const Iterator<T> begin() const
	{
		return Iterator<T>(buffer);
	}

	reversed_iterator<T> rbegin()
	{
		return reversed_iterator<T>(buffer + size_ - 1);
	}

	const reversed_iterator<T> rbegin() const
	{
		return reversed_iterator<T>(buffer + size_ - 1);
	}

	Iterator<T> end()
	{
		return Iterator<T>(buffer + size_);
	}

	const Iterator<T> end() const
	{
		return Iterator<T>(buffer + size_ );
	}

	reversed_iterator<T> rend()
	{
		return reversed_iterator<T>(buffer - 1);
	}

	const reversed_iterator<T> rend() const
	{
		return reversed_iterator<T>(buffer - 1);
	}

	size_t size() const
	{
		return size_;
	}

	bool empty() const
	{
		return (size_ == 0);
	}

	T& operator[](size_t i)
	{
		if (i >= size_)
			throw std::out_of_range ("check operator[]");
		return buffer[i];
	}

	const T& operator[](size_t i) const
	{
		if (i >= size_)
			throw std::out_of_range ("check operator[]");
		return buffer[i];
	}

    void reserve(size_t new_capacity)
    {

		if (capacity_ < new_capacity) {
			T* new_data = alloc_.alloc(new_capacity);
			std::memcpy(new_data, buffer, size_ * sizeof(T));
			std::swap(buffer, new_data);
			alloc_.free_(new_data);
			capacity_ = new_capacity;
		}

    }


	void resize(size_t new_size)
	{
        if (new_size > capacity_) {
            reserve(new_size);
        }

        for(int i = size_; i < new_size; i++)
            alloc_.construct(buffer + i);

        for(int i = new_size; i < size_; i++)
            alloc_.destroy(buffer + i);

        size_ = new_size;
	}

	void push_back(const T& el)
	{
		if (size_ == capacity_)
			reserve(capacity_ * 2);
		alloc_.construct(buffer + size_, el);
		size_++;
	}

	void push_back(T&& el)
	{
		if (size_ == capacity_)
			reserve(capacity_ * 2);
		alloc_.construct(buffer + size_, std::move(el));
		size_++;
	}

	void pop_back()
	{
		resize(size_-1);
	}

	void clear()
	{
		resize(0);
	}

	size_t capacity()
	{
		return capacity_;
	}
};
