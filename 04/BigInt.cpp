#include "BigInt.h"

BigInt::BigInt(const BigInt n) {
    this.data = n.data
    this.len = n.len
    this.sign = n.sign
}

BigInt::BigInt(int n) {
    size_t len = 0;
    if (n > 0) {
        this.sign = 1;
    } else if (n < 0) {
        this.sign = -1
    } else {
        this.sign = 0;
    }
    char *data = (char *) malloc(10 * sizeof(int));
    size_t max_len = 10;
    while (n != 0) {
        char cur = '0' + n % 10;
        n /= 10;
        len++;
        if (len > max_len) {
            char *tmp = data;
            data = (char *) realloc((void *) data, 2 * max_len);
            max_len *= 2;
        }
        data[len - 1] = cur;
    }
    this.data = data;
    this.len = len;
}

BigInt::~BigInt() {
    free(this.data);
}

BigInt operator=(int n) {
    size_t len = 0;
    if (n > 0) {
        this.sign = 1;
    } else if (n < 0) {
        this.sign = -1
    } else {
        this.sign = 0;
    }
    char *data = (char *) malloc(10 * sizeof(int));
    size_t max_len = 10;
    data[0] = '0';
    while (n != 0) {
        char cur = '0' + n % 10;
        n /= 10;
        len++;
        if (len > max_len) {
            char *tmp = data;
            data = (char *) realloc((void *) data, 2 * max_len);
            max_len *= 2;
        }
        data[len - 1] = cur;
    }
    this.data = data;
    this.len = len;
}

BigInt::operator=(const BigInt n) {
    this.len = n.len;
    this.sign = n.sign;
    this.data = n.data;
}

const bool BigInt::operator==(const BigInt c) const {
    if (c.sign == this.sign && c.len == this.len) {
        for (size_t i = 0; i < c.len; i++) {
            if(this.data[i] != c.data[i]) { return false; }
        }
        return true;
    }
    return false;
}

const bool BigInt::operator>(const BigInt c) const {
    if(this.sign != c.sign) { return this.sign > c.sign; }
    if(this.sign == 1) {
        if(this.len != c.len) { return  this.len > c.len; }
        for (size_t i = len - 1; i >= 0; i--) {
            if(this.data[i] != c.data[i]) {
                return (this.data[i] > c.data[i]);
            }
        }
        return false;
    } else if(this.sign == -1) {
        if(this.len != c.len) { return  this.len < c.len; }
        for (size_t i = len - 1; i >= 0; i--) {
            if(this.data[i] != c.data[i]) {
                return (this.data[i] < c.data[i]);
            }
        }
        return false;
    }
    return false;
}

const bool BigInt::operator>=(const BigInt c) const {
    return this.operator>(c) || this.operator==(c);
}

const bool BigInt::operator<(const BigInt c) const {
    return !this.operator>=(c);
}

const bool BigInt::operator<=(const BigInt c) const {
    return !this.operator>(c);
}

const bool BigInt::operator!=(const BigInt c) const {
    return !this.operator==(c);
}

BigInt BigInt::operator+(BigInt c) {
    BigInt tmp(sum(this, c, 1));
    return tmp;
}

BigInt BigInt::operator-(BigInt c) {
    BigInt tmp(sum(this, c, -1));
    return tmp;
}

void BigInt::operator+=(const BigInt c) {
    this.data = sum(this, c, 1).data;
}

void BigInt::operator-=(const BigInt c) {
    this.data = sum(this, c, -1).data;
}

BigInt BigInt::sum(const BigInt main, const BigInt add, const int sign)
{
    int s = main.sign * add.sign * sign;
    if (main.len >= add.len) {
        BigInt tmp(main);
        for (size_t i = 0; i < add.len; i++) {
            tmp.data[i] += s * (add.data[i] - '0');
        }
    } else {
        BigInt tmp(add);
        for (size_t i = 0; i < main.len; i++) {
            tmp.data[i] += s * (main.data[i] - '0');
        }
    }
    tmp.shift();
    return tmp;
}

void BigInt::shift()
{
    char shift = 0;
    for (size_t i = 0; i < this.len; i++) {
        shift = (this.data[i] - '0' + 10) / 10 - 1;
        if (shift == -1) {
            this.data[i] += 10;
        }
    }
}
