#ifndef __VEC_H__
#define __VEC_H__

#include <cstring>
#include <cmath>

template <typename T, int dim>
class Vec {
private:
    T data[dim];

    template<typename U, int D>
    friend Vec<U, D> operator* (U lhs, const Vec<U, D> &rhs);
public:
    Vec();
    Vec(T a1, T a2, T a3); //Change it to specialization 

    T& operator[] (int ind);

    Vec<T, dim>& operator+= (const Vec<T, dim>& rhs);
    Vec<T, dim>& operator-= (const Vec<T, dim>& rhs);
    Vec<T, dim>& operator*= (T rhs);
    Vec<T, dim>& operator/= (T rhs);

    Vec<T, dim> operator+ (const Vec<T, dim>& rhs) const;
    Vec<T, dim> operator- (const Vec<T, dim>& rhs) const;
    Vec<T, dim> operator* (T rhs) const;
    Vec<T, dim> operator/ (T rhs) const;

    Vec<T, dim> operator- () const;

    T operator* (const Vec<T, dim>& rhs) const; //Dot product
    Vec<T, dim> operator^ (const Vec<T, dim>& rhs) const; //Cross product

    T length() const;
    Vec<T, dim> normalize() const;
};

template <typename T, int dim>
Vec<T, dim>::Vec() {
    for (int i = 0; i < dim; i++)
        data[i] = 0;
}

template <typename T, int dim>
Vec<T, dim>::Vec(T a1, T a2, T a3) {
    if (dim != 3)
        throw;
    data[0] = a1;
    data[1] = a2;
    data[2] = a3;
}

template <typename T, int dim>
T& Vec<T, dim>::operator[] (int ind) {
    return data[ind];
}

template <typename T, int dim>
Vec<T, dim>& Vec<T, dim>::operator+= (const Vec<T, dim>& rhs) {
    for (int i = 0; i < dim; i++)
        data[i] += rhs.data[i];
    return *this;
}

template <typename T, int dim>
Vec<T, dim>& Vec<T, dim>::operator-= (const Vec<T, dim>& rhs) {
    for (int i = 0; i < dim; i++)
        data[i] -= rhs.data[i];
    return *this;
}

template <typename T, int dim>
Vec<T, dim>& Vec<T, dim>::operator*= (T rhs) {
    for (int i = 0; i < dim; i++)
        data[i] *= rhs;
    return *this;
}

template <typename T, int dim>
Vec<T, dim>& Vec<T, dim>::operator/= (T rhs) {
    for (int i = 0; i < dim; i++)
        data[i] /= rhs;
    return *this;
}

template <typename T, int dim>
Vec<T, dim> Vec<T, dim>::operator+ (const Vec<T, dim>& rhs) const {
    return Vec<T, dim>(*this) += rhs;
}

template <typename T, int dim>
Vec<T, dim> Vec<T, dim>::operator- (const Vec<T, dim>& rhs) const {
    return Vec<T, dim>(*this) -= rhs;
}

template <typename T, int dim>
Vec<T, dim> Vec<T, dim>::operator* (T rhs) const {
    return Vec<T, dim>(*this) *= rhs;
}

template <typename T, int dim>
Vec<T, dim> Vec<T, dim>::operator/ (T rhs) const {
    return Vec<T, dim>(*this) /= rhs;
}

template <typename T, int dim>
Vec<T, dim> Vec<T, dim>::operator-() const {
    return Vec<T, dim>(*this) * -1;
}

template <typename T, int dim>
T Vec<T, dim>::operator* (const Vec<T, dim>& rhs) const {
    T t = 0;
    for (int i = 0; i < dim; i++)
        t += data[i] * rhs.data[i];
    return t;
}

template <typename T, int dim>
Vec<T, dim> Vec<T, dim>::operator^ (const Vec<T, dim>& rhs) const {
    if (dim != 3)
        throw "NOT_IMPLEMENTED"; //Or not defined, I don't know...
    return Vec<T, dim>(
        data[1] * rhs.data[2] - data[2] * rhs.data[1],
        data[2] * rhs.data[0] - data[0] * rhs.data[2],
        data[0] * rhs.data[1] - data[1] * rhs.data[0]
    );
}

template <typename T, int dim>
T Vec<T, dim>::length() const {
    T t = 0;
    for (int i = 0; i < dim; i++)
        t += data[i] * data[i];
    return sqrt(t);
}

template <typename T, int dim>
Vec<T, dim> Vec<T, dim>::normalize() const {
    return Vec<T, dim>(*this) /= length();
}

template <typename U, int D>
Vec<U, D> operator* (U lhs, const Vec<U, D> &rhs) {
    return rhs * lhs;
}


typedef Vec<int, 3> Vec3i;
typedef Vec<float, 3> Vec3f;
typedef Vec<double, 3> Vec3d;

typedef Vec<int, 4> Vec4i;
typedef Vec<float, 4> Vec4f;
typedef Vec<double, 4> Vec4d;


#endif