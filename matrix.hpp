#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vec.hpp"

template <typename T, int dimX, int dimY>
struct Matrix {
    Vec<T, dimY> columns[dimX];
    
    Matrix() {
        for (int i = 0; i < dimX; i++)
            columns[i] = Vec<T, dimY>();
    }
    Matrix(const std::initializer_list<T> &l) { //...
        assert(l.size() == dimX * dimY);
        int index = 0;
        for (auto&& e : l) {
            (*this)[index / dimX][index % dimX] = e;
            index++;
        }
    }
    Vec<T, dimY>& operator[] (int index) {
        assert(index >= 0 && index < dimX);
        return columns[index];
    }
    const Vec<T, dimY>& operator[] (int index) const {
        assert(index >= 0 && index < dimX);
        return columns[index];
    }
    Matrix<T, dimY, dimX> transpose() const {
        Matrix<T, dimY, dimX> t;
        for (int i = 0; i < dimY; i++)
            for (int j = 0; j < dimX; j++)
                t[i][j] = (*this)[j][i];
        return t;
    }
    Matrix<T, dimX, dimY> inverse() const {
        assert(dimX == dimY);
        throw "NOT_IMPLEMENTED";
    }
    float determinant() const {
        assert(dimX == dimY);
        throw "NOT_IMPLEMENTED";
    }
    Matrix<T, dimX, dimY> pow(int deg) const { //TODO: change this to faster algorithm!
        Matrix<T, dimX, dimY> t(*this);
        for (int i = 1; i < deg; i++)
            t = t * t;
        return t;
    }
};

//Addition and subtraction

template <typename T, int dimX, int dimY>
Matrix<T, dimX, dimY> operator+ (const Matrix<T, dimX, dimY> &lhs, const Matrix<T, dimX, dimY> &rhs) {
    Matrix<T, dimX, dimY> t;
    for (int i = 0; i < dimX; i++)
        for (int j = 0; j < dimY; j++)
            t[i][j] = lhs[i][j] + rhs[i][j];
    return t;
}

template <typename T, int dimX, int dimY>
Matrix<T, dimX, dimY> operator- (const Matrix<T, dimX, dimY> &lhs, const Matrix<T, dimX, dimY> &rhs) {
    Matrix<T, dimX, dimY> t;
    for (int i = 0; i < dimX; i++)
        for (int j = 0; j < dimY; j++)
            t[i][j] = lhs[i][j] - rhs[i][j];
    return t;
}

//Multiply and division by number

template <typename T, int dimX, int dimY>
Matrix<T, dimX, dimY> operator* (const Matrix<T, dimX, dimY> &lhs, const T &rhs) {
    Matrix<T, dimX, dimY> t;
    for (int i = 0; i < dimX; i++)
        for (int j = 0; j < dimY; j++)
            t[i][j] = lhs[i][j] * rhs;
    return t;
}

template <typename T, int dimX, int dimY>
Matrix<T, dimX, dimY> operator/ (const Matrix<T, dimX, dimY> &lhs, const T &rhs) {
    Matrix<T, dimX, dimY> t;
    for (int i = 0; i < dimX; i++)
        for (int j = 0; j < dimY; j++)
            t[i][j] = lhs[i][j] / rhs;
    return t;
}

template <typename T, int dimX, int dimY>
Matrix<T, dimX, dimY> operator* (const T &lhs, const Matrix<T, dimX, dimY> &rhs) {
    return rhs * lhs;
}
template <typename T, int dimX, int dimY>
Matrix<T, dimX, dimY> operator/ (const T &lhs, const Matrix<T, dimX, dimY> &rhs) {
    return rhs / lhs;
}

//Multiply by vector
template <typename T, int dim>
Vec<T, dim> operator* (const Matrix<T, dim, dim> &lhs, const Vec<T, dim> &rhs) {
    Vec<T, dim> t;
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
            t[i] += lhs[i][j] * rhs[j];
    return t;
}

//Multiply by matrix

template <typename T, int dimA, int dimB, int dimO>
Matrix<T, dimA, dimB> operator* (const Matrix<T, dimA, dimO> &lhs, const Matrix<T, dimO, dimB> &rhs) {
    Matrix<T, dimA, dimB> res;
    for (int i = 0; i < dimO; i++)
        res[i] = lhs * rhs[i];
    return res;
}

//Definitions

typedef Matrix<int, 3, 3> Mat3i;
typedef Matrix<float, 3, 3> Mat3f;
typedef Matrix<double, 3, 3> Mat3d;
typedef Matrix<long double, 3, 3> Mat3l;

#endif