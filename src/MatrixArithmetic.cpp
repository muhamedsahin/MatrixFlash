#include "Matrix/Matrix.hpp"
#include <stdexcept>

Matrix Matrix::multiply(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Matris çarpımı için iç boyutlar uyuşmuyor!");
    }

    Matrix result(rows, other.cols);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < other.cols; ++col) {
            float sum = 0.0f;
            for (int index = 0; index < cols; ++index) {
                sum += at(row, index) * other.at(index, col);
            }
            result.at(row, col) = sum;
        }
    }
    return result;
}

Matrix Matrix::add(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matris toplama için boyutlar tam olarak eşit olmalı!");
    }

    Matrix result(rows, cols);
    for (size_t index = 0; index < data.size(); ++index) {
        result.data[index] = data[index] + other.data[index];
    }
    return result;
}

Matrix Matrix::subtract(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matris çıkarma için boyutlar tam olarak eşit olmalı!");
    }

    Matrix result(rows, cols);
    for (size_t index = 0; index < data.size(); ++index) {
        result.data[index] = data[index] - other.data[index];
    }
    return result;
}

Matrix Matrix::elementwiseMultiply(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matris elementel çarpım için boyutlar tam olarak eşit olmalı!");
    }

    Matrix result(rows, cols);
    for (size_t index = 0; index < data.size(); ++index) {
        result.data[index] = data[index] * other.data[index];
    }
    return result;
}
