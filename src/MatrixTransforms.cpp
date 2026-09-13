#include "Matrix/Matrix.hpp"
#include <algorithm>
#include <stdexcept>

Matrix Matrix::identity(int size) {
    Matrix result(size, size);
    result.setIdentity();
    return result;
}

Matrix Matrix::ones(int rows, int cols) {
    if (rows < 0 || cols < 0) {
        throw std::invalid_argument("Matris boyutları negatif olamaz!");
    }
    Matrix result(rows, cols);
    std::fill(result.data.begin(), result.data.end(), 1.0);
    return result;
}

void Matrix::setIdentity() {
    if (rows != cols) {
        throw std::invalid_argument("Birim matris sadece kare matrislerde oluşturulabilir!");
    }

    std::fill(data.begin(), data.end(), 0.0);
    for (int index = 0; index < rows; ++index) {
        data[index * cols + index] = 1.0;
    }
}

void Matrix::zero() {
    std::fill(data.begin(), data.end(), 0.0);
}

Matrix Matrix::zeros(int rows, int cols) {
    Matrix result(rows, cols);
    result.zero();
    return result;
}

void Matrix::flatten() {
    cols = rows * cols;
    rows = 1;
}