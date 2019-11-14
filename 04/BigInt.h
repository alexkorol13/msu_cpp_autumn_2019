#ifndef BIGINT_H_INCLUDED
#define BIGINT_H_INCLUDED

class BigInt {
    BigInt(int n);
    BigInt(const BigInt& n);
    ~BigInt();
    const bool operator==(BigInt c) const;
    const bool operator!=(BigInt c) const;
    const bool operator<=(BigInt c) const;
    const bool operator>=(BigInt c) const;
    const bool operator>(BigInt c) const;
    const bool operator<(BigInt c) const;
    BigInt operator+(BigInt c);
    BigInt operator+=(BigInt c);
    BigInt operator-=(BigInt c);
    BigInt operator-(BigInt c);
    BigInt sum(BigInt a, BigInt b, int s);
    void shift();
    BigInt operator=(BigInt c);
    BigInt operator=(int c);
};

#endif // BIGINT_H_INCLUDED
