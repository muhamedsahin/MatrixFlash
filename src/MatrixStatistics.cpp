#include "Matrix/Matrix.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <stdexcept>

void Matrix::negate() {
    std::transform(data.begin(), data.end(), data.begin(), [](double value) {
        return -value;
    });
}

double Matrix::mean() const {
    if (data.empty()) {
        throw std::domain_error("Boş matrisin ortalaması hesaplanamaz!");
    }
    return std::reduce(data.begin(), data.end(), 0.0) / data.size();
}

double Matrix::l1_norm() const {
    return std::transform_reduce(
        data.begin(), data.end(), 0.0, std::plus<>(),
        [](double value) { return std::abs(value); }
    );
}

double Matrix::l2_norm() const {
    double sum_squared = std::transform_reduce(
        data.begin(), data.end(), 0.0, std::plus<>(),
        [](double value) { return value * value; }
    );
    return std::sqrt(sum_squared);
}

double Matrix::trace() const {
    if (rows != cols) {
        throw std::invalid_argument("Matrix en boy boyutları birbirine eşit olması lazım");
    }

    double sum = 0.0;
    for (int index = 0; index < rows; ++index) {
        sum += data[index * (rows + 1)];
    }
    return sum;
}