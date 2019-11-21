#pragma once

#include <vector>
#include <sstream>

using namespace std;

void process(vector<string>& strings) {}

template <class T>
void process(vector<string>& strings, const T& val)
{
    stringstream t;
    t << val;
    strings.push_back(t.str());
}


template <class T, class... Args>
void process(vector<string>& res, const T& val, const Args&... args)
{
    stringstream t;
    t << val;
    res.push_back(t.str());
    process(res, args...);
}

template <class ... ArgsT>
string format(const string& str, const ArgsT&... args)
{
    stringstream res;
    vector<string> strings;
    process(strings, args...);
    size_t len = str.size(), vec_size = strings.size();
    size_t i = 0;
    while (i < len) {
        if (str[i] == '{') {
            if (i + 1 < len && (isdigit(str[i + 1]))) {
                int ind = 0;
                i++;
                while (i < len && isdigit(str[i])) {
                    ind = ind * 10 + str[i] - '0';
                    i++;
                }
                if (i >= len || str[i] != '}') {
                    throw runtime_error("context mismatch\n");
                }
                if (ind >= vec_size) {
                    throw runtime_error("argument index out of range\n");
                }
                res << strings[ind];
            } else {
                throw runtime_error("context mismatch\n");
            }
        } else if (str[i] == '}') {
            throw runtime_error("context mismatch\n");
        } else {
            res << str[i];
        }
        i++;
    }
    return res.str();
}
