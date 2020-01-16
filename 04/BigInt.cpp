#include <iostream>
#include <math.h>
#include "BigInt.h"

using namespace std;

BigInt::BigInt() : size(1), is_neg(false), is_zero(true) {
    arr = new char[1]();
}

BigInt::BigInt(long long n) {
    is_neg = false;
    if (n < 0) {
        is_neg = true;
        n = -n;
    }
    is_zero = false;
    if (n == 0) {
        arr = new char[1]();
        size = 1;
        is_zero = true;
    } else {
        size = 0;
        arr = new char[20]();
        while (n > 0) {
            arr[size] = n % 10;
            size++;
            n /= 10;
        }
    }
}

BigInt::BigInt(char *m, size_t s, bool sign) : arr(m), size(s), is_neg(sign) {
    is_zero = ((size == 1) && (arr[0] == 0));
}

BigInt::BigInt(const BigInt &val) : arr(new char[val.size]), is_neg(val.is_neg), size(val.size), is_zero(val.is_zero) {
    std::copy(val.arr, val.arr + size, arr);
}

BigInt::BigInt(BigInt &&val) : size(val.size), is_neg(val.is_neg), is_zero(val.is_zero) {
    delete[] arr;
    arr = val.arr;
    val.arr = nullptr;
    val.size = 0;
}

BigInt::~BigInt() {
    delete[] arr;
}

std::ostream &operator<<(std::ostream &out, const BigInt &val) {
    if (val.is_zero) {
        out << "0";
        return out;
    }
    if (val.is_neg) {
        out << '-';
    }
    for (int i = val.size - 1; i >= 0; i--) {
        out << int(val.arr[i]);
    }
    return out;
}

BigInt &BigInt::operator=(const BigInt &val) {
    if (val == *this) {
        return *this;
    }
    char *tmp = new char[val.size];
    delete[] arr;
    arr = tmp;
    size = val.size;
    is_zero = val.is_zero;
    std::copy(val.arr, val.arr + size, arr);
    return *this;
}

BigInt &BigInt::operator=(BigInt &&val) {
    if (*this == val) {
        return *this;
    }
    delete[] arr;
    arr = val.arr;
    size = val.size;
    is_neg = val.is_neg;
    is_zero = val.is_zero;
    val.arr = nullptr;
    val.size = 0;
    return *this;
}

bool BigInt::operator==(const BigInt &val) const {
    if (size != val.size) {
        return false;
    }
    if (is_zero && val.is_zero) {
        return true;
    }
    if (is_neg != val.is_neg) {
        return false;
    }
    for (int i = size - 1; i >= 0; i--) {
        if (arr[i] != val.arr[i]) {
            return false;
        }
    }
    return true;
}

bool BigInt::operator!=(const BigInt &val) const {
    return !(*this == val);
}

bool BigInt::operator<=(const BigInt &val) const {
    if (*this == val) {
        return true;
    }
    if (val.is_neg && !is_neg) {
        return false;
    }
    if (!val.is_neg && is_neg) {
        return true;
    }
    if (!val.is_neg && !is_neg) {
        if (val.size > size) {
            return true;
        }
        if (val.size < size) {
            return false;
        }
        for (int i = size - 1; i >= 0; i--) {
            if (arr[i] > val.arr[i]) {
                return false;
            } else if (arr[i] < val.arr[i]) {
                return true;
            }
        }
        return true;
    } else if (val.is_neg && is_neg) {
        if (val.size > size) {
            return false;
        }
        if (val.size < size) {
            return true;
        }
        for (int i = size - 1; i >= 0; i--) {
            if (arr[i] > val.arr[i]) {
                return true;
            } else if (arr[i] < val.arr[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool BigInt::operator>(const BigInt &val) const {
    return !(*this <= val);
}

bool BigInt::operator>=(const BigInt &val) const {
    return (*this > val || *this == val);
}

bool BigInt::operator<(const BigInt &val) const {
    return !(*this >= val);
}

BigInt BigInt::operator+(const BigInt& val) const {
    if (!is_neg && val.is_neg) {
        auto tmp = -val;
        return (*this).operator-(tmp);
    }
    if (is_neg && !val.is_neg) {
        auto tmp = -(*this);
        return val.operator-(tmp);
    }
    int max_size = 0;
    int min_size = 0;
    if (size >= val.size) {
        max_size = size;
        min_size = val.size;
    } else {
        max_size = val.size;
        min_size = size;
    }
    char to_add = 0;
    char *res = new char[max_size + 1]();
    for (int i = 0; i < min_size; i++) {
        res[i] = (arr[i] + val.arr[i] + to_add) % 10;
        to_add = (arr[i] + val.arr[i] + to_add) / 10;
    }
    for (int i = min_size; i < max_size; i++) {
        if (max_size == size) {
            res[i] = (arr[i] + to_add) % 10;
            to_add = (arr[i] + to_add) / 10;
        } else {
            res[i] = (val.arr[i] + to_add) % 10;
            to_add = (val.arr[i] + to_add) / 10;
        }
    }
    int total = max_size;
    if (to_add > 0) {
        res[max_size] = to_add;
        total++;
    }
    return BigInt(res, total, is_neg);
}

BigInt BigInt::operator-(const BigInt& val) const {
    if (is_neg && val.is_neg) {
        auto tmp1 = -(*this);
        auto tmp2 = -val;
        return -(tmp1 - tmp2);
    }
    if (is_neg && !val.is_neg) {
        auto tmp = -(*this);
        return -(val.operator+(tmp));
    }
    if (!is_neg && val.is_neg) {
        auto tmp = -val;
        return (*this).operator+(tmp);
    }
    if (size > val.size) {
        char *res = new char[size]();
        int to_add = 0;
        for (int i = 0; i < val.size; i++) {
            res[i] = arr[i] - val.arr[i] - to_add;
            to_add = 0;
            if (res[i] < 0) {
                res[i] += 10;
                to_add = 1;
            }
        }
        for (int i = val.size; i < size; i++) {
            res[i] = arr[i] - to_add;
            to_add = 0;
            if (res[i] < 0) {
                res[i] += 10;
                to_add = 1;
            }
        }
        int total = size - 1;
        for (; res[total] == 0; total--);
        return BigInt(res, total + 1, false);
    } else if (val.size > size) {
        char *res = new char[val.size]();
        int to_add = 0;
        for (int i = 0; i < size; i++) {
            res[i] = val.arr[i] - arr[i] - to_add;
            to_add = 0;
            if (res[i] < 0) {
                res[i] += 10;
                to_add = 1;
            }
        }
        for (int i = size; i < val.size; i++) {
            res[i] = val.arr[i] - to_add;
            to_add = 0;
            if (res[i] < 0) {
                res[i] += 10;
                to_add = 1;
            }
        }
        int total = val.size - 1;
        for (; res[total] == 0; total--);
        return BigInt(res, total + 1, true);
    } else {
        if (*this == val) {
            return BigInt(0);
        }
        if (*this > val) {
            char *res = new char[size]();
            int to_add = 0;
            for (int i = 0; i < size; i++) {
                res[i] = arr[i] - val.arr[i] - to_add;
                to_add = 0;
                if (res[i] < 0) {
                    res[i] += 10;
                    to_add = 1;
                }
            }
            int total = size - 1;
            for (; res[total] == 0; total--);
            return BigInt(res, total + 1, false);
        } else {
            char *res = new char[size]();
            int to_add = 0;
            for (int i = 0; i < size; i++) {
                res[i] = val.arr[i] - arr[i] - to_add;
                to_add = 0;
                if (res[i] < 0) {
                    res[i] += 10;
                    to_add = 1;
                }
            }
            int total = size - 1;
            for (; res[total] == 0; total--);
            return BigInt(res, total + 1, true);
        }
    }
}

BigInt BigInt::operator-() const {
    BigInt tmp(*this);
    tmp.is_neg = !tmp.is_neg;
    return tmp;
}
