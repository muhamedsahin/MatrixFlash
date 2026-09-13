#include "Matrix/Matrix.hpp"

#include <iomanip>
#include <iostream>

void print_matrix(const Matrix& matrix, const char* title) {
    std::cout << title << "\n";
    for (int row = 0; row < matrix.row_s(); ++row) {
        for (int col = 0; col < matrix.col_s(); ++col) {
            std::cout << std::setw(10) << matrix.at(row, col) << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    Matrix matrix(2, 2);
    matrix.at(0, 0) = 4.0;
    matrix.at(0, 1) = 7.0;
    matrix.at(1, 0) = 2.0;
    matrix.at(1, 1) = 6.0;

    std::cout << "determinant: " << matrix.determinant() << '\n';
    std::cout << "trace: " << matrix.trace() << '\n';
    print_matrix(matrix.Inverse(), "inverse:");

    LUDecomposition lu = matrix.lu_decompose();
    print_matrix(lu.L, "L:");
    print_matrix(lu.U, "U:");
    print_matrix(lu.P, "P:");

    Matrix diagonal = Matrix::zeros(2, 2);
    diagonal.at(0, 0) = 5.0;
    diagonal.at(1, 1) = 2.0;
    auto eigen = diagonal.power_iteration();
    std::cout << "dominant eigenvalue: " << std::setprecision(6)
              << eigen.first << '\n';
    print_matrix(eigen.second, "eigenvector:");

    return 0;
}
