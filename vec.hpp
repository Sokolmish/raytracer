#ifndef __VEC_H__
#define __VEC_H__

#include <cstring>
#include <cmath>
#include <cassert>
#include <initializer_list>

template <typename T, int dim>
struct Vec {
    T data[dim];
    //Constructors
    Vec() {
        std::memset(data, 0, dim);
    }
    Vec(const std::initializer_list<T> &l) { //...
        assert(l.size == dim);
        int index = 0;
        for (auto&& e : l)
            operator[] (index++) = e;
    }
    //Indexation
    T& operator[] (int index) {
        assert(index < dim && index >= 0);
        return data[index];
    }
    const T& operator[] (int index) const {
        assert(index < dim && index >= 0);
        return data[index];
    }
    //Length and normalization
    float length() const {
        T t = 0;
        for (int i = 0; i < dim; i++)
            t += data[i] * data[i];
        return sqrtf(t);
    }
    Vec<T, dim> normalize() const {
        return (*this) / length();
    }
};

template <typename T>
struct Vec<T, 2> {
    T x, y;
    //Constructors
    Vec() {
        x = y = 0;
    }
    Vec(const std::initializer_list<T> &l) { //...
        assert(l.size == 3);
        int index = 0;
        for (auto&& e : l)
            operator[] (index++) = e;
    }
    Vec(const T &x, const T &y, const T &z) {
        this->x = x;
        this->y = y;
    }
    //Indexation
    T& operator[] (int index) {
        assert(index < 2 && index >= 0);
        if (index == 0)
            return x;
        else //index == 1
            return y;
    }
    const T& operator[] (int index) const {
        assert(index < 2 && index >= 0);
        if (index == 0)
            return x;
        else //index == 1
            return y;
    }
    //Length and normalization
    float length() const {
        return sqrtf(x * x + y * y);
    }
    Vec<T, 2> normalize() const {
        return (*this) / length();
    }
};

template <typename T>
struct Vec<T, 3> {
    T x, y, z;
    //Constructors
    Vec() {
        x = y = z = 0;
    }
    Vec(const std::initializer_list<T> &l) { //...
        assert(l.size == 3);
        int index = 0;
        for (auto&& e : l)
            operator[] (index++) = e;
    }
    Vec(const T &x, const T &y, const T &z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    //Indexation
    T& operator[] (int index) {
        assert(index < 3 && index >= 0);
        if (index == 0)
            return x;
        else if (index == 1)
            return y;
        else //index == 2
            return z;
    }
    const T& operator[] (int index) const {
        assert(index < 3 && index >= 0);
        if (index == 0)
            return x;
        else if (index == 1)
            return y;
        else //index == 2
            return z;
    }
    //Length and normalization
    float length() const {
        return sqrtf(x * x + y * y + z * z);
    }
    Vec<T, 3> normalize() const {
        return (*this) / length();
    }
    //Cross product
    Vec<T, 3> operator^ (const Vec<T, 3> &rhs) const {
        return Vec<T, 3>(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        );
    }
};

template <typename T>
struct Vec<T, 4> {
    T x, y, z, w;
    //Constructors
    Vec() {
        x = y = z = w = 0;
    }
    Vec(const std::initializer_list<T> &l) { //...
        assert(l.size == 3);
        int index = 0;
        for (auto&& e : l)
            operator[] (index++) = e;
    }
    Vec(const T &x, const T &y, const T &z, const T &w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    //Indexation
    T& operator[] (int index) {
        assert(index < 4 && index >= 0);
        if (index == 0)
            return x;
        else if (index == 1)
            return y;
        else if (index == 2)
            return z;
        else //index == 3
            return w;
    }
    const T& operator[] (int index) const {
        assert(index < 4 && index >= 0);
        if (index == 0)
            return x;
        else if (index == 1)
            return y;
        else if (index == 2)
            return z;
        else //index == 3
            return w;
    }
    //Length and normalization
    float length() const {
        return sqrtf(x * x + y * y + z * z + w * w);
    }
    Vec<T, 4> normalize() const {
        return (*this) / length();
    }
};

//Addition and Subtraction
template <typename T, int dim>
Vec<T, dim> operator+ (const Vec<T, dim> &lhs, const Vec<T, dim> &rhs) {
    Vec<T, dim> t;
    for (int i = 0; i < dim; i++)
        t[i] = lhs[i] + rhs[i];
    return t;
}

template <typename T, int dim>
Vec<T, dim> operator- (const Vec<T, dim> &lhs, const Vec<T, dim> &rhs) {
    Vec<T, dim> t;
    for (int i = 0; i < dim; i++)
        t[i] = lhs[i] - rhs[i];
    return t;
}

template <typename T, int dim>
Vec<T, dim> operator- (const Vec<T, dim> &rhs) {
    return Vec<T, dim>(rhs) * (T) (-1);
}

//Muliply and division by number
template <typename T, int dim>
Vec<T, dim> operator* (const Vec<T, dim> &lhs, const T &rhs) {
    Vec<T, dim> t = Vec<T, dim>(lhs);
    for (int i = 0; i < dim; i++)
        t[i] *= rhs;
    return t;
}

template <typename T, int dim>
Vec<T, dim> operator/ (const Vec<T, dim> &lhs, const T &rhs) {
    Vec<T, dim> t = Vec<T, dim>(lhs);
    for (int i = 0; i < dim; i++)
        t[i] /= rhs;
    return t;
}

template <typename T, int dim>
Vec<T, dim> operator* (const T &lhs, const Vec<T, dim> &rhs) {
    return rhs * lhs;
}

template <typename T, int dim>
Vec<T, dim> operator/ (const T &lhs, const Vec<T, dim> &rhs) {
    Vec<T, dim> t = Vec<T, dim>();
    for (int i = 0; i < dim; i++)
        t[i] = rhs / lhs[i];
    return t;
}

//Dot product
template <typename T, int dim>
T operator* (const Vec<T, dim> &lhs, const Vec<T, dim> &rhs) {
    T t = 0;
    for (int i = 0; i < dim; i++)
        t += lhs[i] * rhs[i];
    return t;
}

//Definitions

typedef Vec<int, 2> Vec2i;
typedef Vec<float, 2> Vec2f;
typedef Vec<double, 2> Vec2d;
typedef Vec<long double, 2> Vec2l;

typedef Vec<int, 3> Vec3i;
typedef Vec<float, 3> Vec3f;
typedef Vec<double, 3> Vec3d;
typedef Vec<long double, 3> Vec3l;

typedef Vec<int, 4> Vec4i;
typedef Vec<float, 4> Vec4f;
typedef Vec<double, 4> Vec4d;
typedef Vec<long double, 4> Vec4l;

#endif