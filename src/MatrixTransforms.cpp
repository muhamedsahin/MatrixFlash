#include "Matrix/Matrix.hpp"
#include <algorithm>
#include <random>
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

Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            result.data[col * rows + row] = data[row * cols + col];
        }
    }
    return result;
}

void Matrix::randomize(double min_value, double max_value) {
    if (min_value > max_value) {
        throw std::invalid_argument("Rastgele deger araligi gecersiz!");
    }

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<double> distribution(min_value, max_value);
    for (double& value : data) {
        value = distribution(generator);
    }
}