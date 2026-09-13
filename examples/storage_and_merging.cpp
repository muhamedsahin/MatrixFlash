#include "Matrix/Matrix.hpp"

#include <cstdio>
#include <iostream>

void print_matrix(const Matrix& matrix, const char* title) {
    std::cout << title << "\n";
    for (int row = 0; row < matrix.row_s(); ++row) {
        for (int col = 0; col < matrix.col_s(); ++col) {
            std::cout << matrix.at(row, col) << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    Matrix first(2, 2);
    first.at(0, 0) = 1.0;
    first.at(0, 1) = 2.0;
    first.at(1, 0) = 3.0;
    first.at(1, 1) = 4.0;

    Matrix second = Matrix::identity(2);
    Matrix joined = first.concatHorizontal(second);
    print_matrix(joined, "Horizontal concat:");
    print_matrix(joined.slice(0, 2, 1, 3), "Slice:");

    const char* filename = "matrix_example.bin";
    joined.saveBinary(filename);
    Matrix loaded = Matrix::loadBinary(filename);
    print_matrix(loaded, "Loaded from binary:");
    std::remove(filename);

    return 0;
}
