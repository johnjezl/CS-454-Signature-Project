//
// Created by Student on 11/23/25.
//
#pragma once
#include <stdexcept>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

//The code in this file was created by ChatGPT

// Big integer type from Boost
using BigInt = boost::multiprecision::cpp_int;

// Euclidean gcd for BigInt
inline BigInt big_gcd(BigInt a, BigInt b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        BigInt r = a % b;
        a = b;
        b = r;
    }
    return a;
}



struct Rational {
    BigInt num; // numerator
    BigInt den; // denominator > 0

    // Construct from normal integers
    Rational(long long n = 0, long long d = 1) : num(n), den(d) {
        if (den == 0) throw std::runtime_error("Zero denominator");
        normalize();
    }

    // Optional: construct directly from BigInt
    Rational(const BigInt& n, const BigInt& d = 1) : num(n), den(d) {
        if (den == 0) throw std::runtime_error("Zero denominator");
        normalize();
    }

    // Simplify fraction
    void normalize() {
        if (den < 0) { den = -den; num = -num; }

        if (num == 0) {  // canonical zero
            den = 1;
            return;
        }

        BigInt g = big_gcd(num, den);
        if (g != 0) {
            num /= g;
            den /= g;
        }
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


