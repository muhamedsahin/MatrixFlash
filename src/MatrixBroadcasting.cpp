//
// Created by muham on 12.09.2026.
//

#include "Matrix/Matrix.hpp"
#include <stdexcept>

Matrix Matrix::addBroadcast(const Matrix& other) const {
    if (other.rows == 1 && other.cols == this->cols) {
        Matrix result(this->rows, this->cols);
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->cols; ++j) {
                result.data[i * this->cols + j] = this->data[i * this->cols + j] + other.data[j];
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
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                result.at(row, col) = at(row, col) * other.at(0, col);
            }
        }
        return result;
    }
    if (rows == other.rows && cols == other.cols) {
        return elementwiseMultiply(other);
    }
    throw std::invalid_argument("Hata: Broadcasting boyutlari uyusmuyor!");
}