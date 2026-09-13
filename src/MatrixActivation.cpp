#include "Matrix/Matrix.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>

void Matrix::softmax() {
    if (cols == 0) {
        return;
    }

    for (int row = 0; row < rows; ++row) {
        auto row_begin = data.begin() + row * cols;
        auto row_end = row_begin + cols;
        double max_value = *std::max_element(row_begin, row_end);
        double sum = std::transform_reduce(
            row_begin, row_end, 0.0f, std::plus<>(),
            [max_value](double value) { return std::exp(value - max_value); }
        );
        double inverse_sum = 1.0f / sum;
        std::transform(
            row_begin, row_end, row_begin,
            [max_value, inverse_sum](double value) {
                return std::exp(value - max_value) * inverse_sum;
            }
        );
    }
}

Matrix Matrix::NewSoftMax() const {
    Matrix result(rows, cols);
    if (cols == 0) {
        return result;
    }

    for (int row = 0; row < rows; ++row) {
        auto row_begin = data.begin() + row * cols;
        auto row_end = row_begin + cols;
        auto result_begin = result.data.begin() + row * cols;
        double max_value = *std::max_element(row_begin, row_end);
        double sum = std::transform_reduce(
            row_begin, row_end, 0.0f, std::plus<>(),
            [max_value](double value) { return std::exp(value - max_value); }
        );
        double inverse_sum = 1.0f / sum;
        std::transform(
            row_begin, row_end, result_begin,
            [max_value, inverse_sum](double value) {
                return std::exp(value - max_value) * inverse_sum;
            }
        );
    }
    return result;
}
