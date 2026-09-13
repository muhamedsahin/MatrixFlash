#include "Matrix/Matrix.hpp"
#include "obj_console.h"
#include <stdexcept>
#include <utility>

Matrix::Matrix(int rows, int cols) {
    if (rows < 0 || cols < 0) {
        throw std::invalid_argument("Matris boyutları negatif olamaz!");
    }
    this->rows = rows;
    this->cols = cols;
    data.resize(rows * cols);
}

bool Matrix::isValidIndex(int row_num, int col_num) const {
    return row_num >= 0 && row_num < rows && col_num >= 0 && col_num < cols;
}

double& Matrix::at(int row, int col) {
    if (!isValidIndex(row, col)) {
        throw std::out_of_range("Girilen Col ve Row kısımları oluşturulan matris ile uyuşmamaktadır");
    }
    return data[row * cols + col];
}

const double& Matrix::at(int row, int col) const {
    if (!isValidIndex(row, col)) {
        throw std::out_of_range("Girilen Col ve Row kısımları oluşturulan matris ile uyuşmamaktadır");
    }
    return data[row * cols + col];
}

void Matrix::print(Console& console) const {
    for (int row = 0; row < rows; ++row) {
        console.print("[");
        for (int col = 0; col < cols; ++col) {
            console.print(data[row * cols + col], (col < cols - 1) ? "," : "");
        }
        console.log("]");
    }
}

Matrix::Matrix(const Matrix& other)
    : rows(other.rows), cols(other.cols), data(other.data) {
}

Matrix::Matrix(Matrix&& other) noexcept
    : rows(other.rows), cols(other.cols), data(std::move(other.data)) {
    other.rows = 0;
    other.cols = 0;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        rows = other.rows;
        cols = other.cols;
        data = other.data;
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        rows = other.rows;
        cols = other.cols;
        data = std::move(other.data);
        other.rows = 0;
        other.cols = 0;
    }
    return *this;
}

Matrix Matrix::clone() const {
    return Matrix(*this);
}

int Matrix::row_s() const {
    return rows;
}

int Matrix::col_s() const {
    return cols;
}

int Matrix::size() const {
    return rows * cols;
}

bool Matrix::isSquare() const {
    return rows == cols;
}
