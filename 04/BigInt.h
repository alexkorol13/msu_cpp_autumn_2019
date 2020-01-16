#pragma once

class BigInt {
    char *arr;
    size_t size;
    bool is_zero;
    bool is_neg;
    BigInt(char *m, size_t s, bool sign);
public:
    BigInt(long long x);
    BigInt();
    BigInt(const BigInt &val);
    BigInt(BigInt&& val);
    ~BigInt();
    BigInt &operator=(const BigInt &val);
    BigInt &operator=(BigInt&& val);
    friend std::ostream& operator<<(std::ostream& out, const BigInt& val);
    BigInt operator+(const BigInt& val) const;
    BigInt operator-(const BigInt& val) const;
    BigInt operator-() const;
    bool operator==(const BigInt& val) const;
    bool operator!=(const BigInt& val) const;
    bool operator<(const BigInt& val) const;
    bool operator>(const BigInt& val) const;
    bool operator>=(const BigInt& val) const;
    bool operator<=(const BigInt& val) const;
};
