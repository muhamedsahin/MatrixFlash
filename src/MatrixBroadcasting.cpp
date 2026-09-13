//
// Created by muham on 12.09.2026.
//

#include "Matrix/Matrix.hpp"
#include <stdexcept>

Matrix Matrix::addBroadcast(const Matrix& other) const {
    if (other.rows == 1 && other.cols == this->cols) {
        Matrix result(this->rows, this->cols);
        const double* a = this->data.data();
        const double* b = other.data.data();
        double* c = result.data.data();
        const int cols = this->cols;

        for (int i = 0; i < this->rows; ++i) {
            const size_t base = (size_t)i * cols;
            for (int j = 0; j < cols; ++j) {
                c[base + j] = a[base + j] + b[j]; // b satırı yayılır
            }
        }
        return result;
    }
    if (this->rows == other.rows && this->cols == other.cols) {
        return this->add(other);
    }

    throw std::invalid_argument("Hata: Broadcasting boyutlari uyusmuyor!");
}

Matrix Matrix::multiplyBroadcast(const Matrix& other) const {
    if (other.rows == 1 && other.cols == cols) {
        Matrix result(rows, cols);
        const double* a = data.data();
        const double* b = other.data.data();
        double* c = result.data.data();

        for (int row = 0; row < rows; ++row) {
            const size_t base = (size_t)row * cols;
            for (int col = 0; col < cols; ++col) {
                c[base + col] = a[base + col] * b[col];
            }
        }
        return result;
    }
    if (rows == other.rows && cols == other.cols) {
        return elementwiseMultiply(other);
    }
    throw std::invalid_argument("Hata: Broadcasting boyutlari uyusmuyor!");
}