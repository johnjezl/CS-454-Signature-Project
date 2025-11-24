//
// Created by Student on 11/23/25.
//
#pragma once
#include <numeric>
#include <stdexcept>
#include <iostream>

//Created by ChatGPT

struct Rational {
    long long num; // numerator
    long long den; // denominator > 0

    Rational(long long n = 0, long long d = 1) : num(n), den(d) {
        if (den == 0) throw std::runtime_error("Zero denominator");
        normalize();
    }

    //simplifies fraction
    void normalize() {
        if (den < 0) { den = -den; num = -num; }
        long long g = std::gcd(num, den);
        if (g != 0) { num /= g; den /= g; }
    }
};

// Arithmetic
inline Rational operator+(const Rational& a, const Rational& b) {
    return Rational(a.num * b.den + b.num * a.den, a.den * b.den);
}
inline Rational operator-(const Rational& a, const Rational& b) {
    return Rational(a.num * b.den - b.num * a.den, a.den * b.den);
}
inline Rational operator*(const Rational& a, const Rational& b) {
    return Rational(a.num * b.num, a.den * b.den);
}
inline Rational operator/(const Rational& a, const Rational& b) {
    if (b.num == 0) throw std::runtime_error("Division by zero Rational");
    return Rational(a.num * b.den, a.den * b.num);
}

inline bool operator==(const Rational& a, const Rational& b) {
    return a.num == b.num && a.den == b.den;
}
inline bool operator!=(const Rational& a, const Rational& b) {
    return !(a == b);
}

inline bool isZero(const Rational& r) {
    return r.num == 0;
}

inline std::ostream& operator<<(std::ostream& os, const Rational& r) {
    if (r.den == 1) os << r.num;
    else            os << r.num << "/" << r.den;
    return os;
}


