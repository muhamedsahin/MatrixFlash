#include "Matrix/Matrix.hpp"

#include <iostream>

void print_matrix(const Matrix& matrix, const char* title) {
    std::cout << title << " (" << matrix.row_s() << "x" << matrix.col_s() << ")\n";
    for (int row = 0; row < matrix.row_s(); ++row) {
        for (int col = 0; col < matrix.col_s(); ++col) {
            std::cout << matrix.at(row, col) << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    Matrix weights(2, 3);
    weights.at(0, 0) = 1.0;
    weights.at(0, 1) = 2.0;
    weights.at(0, 2) = 3.0;
    weights.at(1, 0) = 4.0;
    weights.at(1, 1) = 5.0;
    weights.at(1, 2) = 6.0;

    Matrix input(3, 1);
    input.at(0, 0) = 10.0;
    input.at(1, 0) = 20.0;
    input.at(2, 0) = 30.0;

    print_matrix(weights, "Weights");
    print_matrix(weights.multiply(input), "Weights * input");
    print_matrix(weights.add(Matrix::ones(2, 3)), "Weights + ones");
    print_matrix(weights.slice(0, 2, 1, 3), "Last two columns");

    return 0;
}
