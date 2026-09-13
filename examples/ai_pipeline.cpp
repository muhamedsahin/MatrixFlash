#include "Matrix/Matrix.hpp"

#include <iomanip>
#include <iostream>

void print_matrix(const Matrix& matrix, const char* title) {
    std::cout << title << "\n";
    for (int row = 0; row < matrix.row_s(); ++row) {
        for (int col = 0; col < matrix.col_s(); ++col) {
            std::cout << std::fixed << std::setprecision(4)
                      << matrix.at(row, col) << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    Matrix inputs(2, 3);
    inputs.at(0, 0) = 1.0;
    inputs.at(0, 1) = 2.0;
    inputs.at(0, 2) = 3.0;
    inputs.at(1, 0) = 2.0;
    inputs.at(1, 1) = 1.0;
    inputs.at(1, 2) = 0.5;

    Matrix bias(1, 3);
    bias.at(0, 0) = 0.5;
    bias.at(0, 1) = 1.0;
    bias.at(0, 2) = -0.5;

    Matrix logits = inputs.addBroadcast(bias);
    Matrix probabilities = logits.NewSoftMax();
    Matrix scaled = probabilities.multiplyBroadcast(Matrix::ones(1, 3));

    print_matrix(logits, "Logits after broadcast bias:");
    print_matrix(probabilities, "Softmax probabilities:");
    print_matrix(scaled, "Element-wise broadcast scaling:");

    return 0;
}
