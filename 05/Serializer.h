#pragma once
#include <cctype>
#include <iostream>

template <class T1, class T2>
struct IsSame
{
    static constexpr bool value = false;
};

template <class T>
struct IsSame<T, T>
{
    static constexpr bool value = true;
};

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : _out(out)
    {}

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::ostream& _out;

    template <class T>
    Error process(T&& val)
    {
        return mySave(val);
    }

    Error mySave(uint64_t val)
    {
        _out << val << Separator;
        return Error::NoError;
    }

    Error mySave(bool val)
    {
        _out << (val ? "true" : "false") << Separator;
        return Error::NoError;
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        mySave(val);
        return process(std::forward<Args>(args)...);
    }
};

class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in)
        : _in(in) {}

    template <class T>
    Error load(T &object)
    {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(args...);
    }

private:
    std::istream& _in;

    Error myLoad(uint64_t& val)
    {
        std::string inp;
        _in >> inp;
        try {
            if (!isdigit(inp[0])) {
                return Error::CorruptedArchive;
            }
            val = std::stoull(inp);
            return Error::NoError;
        } catch (const std::exception& e) {
            return Error::CorruptedArchive;
        }
    }

    Error myLoad(bool& val)
    {
        std::string tmp;
        _in >> tmp;
        if (tmp == "true") {
            val = true;
            return Error::NoError;
        } else if (tmp == "false") {
            val = false;
            return Error::NoError;
        }
        return Error::CorruptedArchive;
    }

    template <class T, class... Args>
    Error process(T&& val, Args&&... args)
    {
        auto load_res = myLoad(val);
        if (load_res == Error::CorruptedArchive) {
            return load_res;
        }
        return process(std::forward<Args>(args)...);
    }

    template <class T>
    Error process(T&& val)
    {
        return myLoad(val);
    }
};
