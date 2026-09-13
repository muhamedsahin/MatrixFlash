#include "Matrix/Matrix.hpp"
#include "obj_console.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

constexpr double epsilon = 1e-9;

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void require_close(double actual, double expected, const std::string& message) {
    if (std::abs(actual - expected) > epsilon) {
        throw std::runtime_error(message);
    }
}

void require_matrix(const Matrix& matrix, int rows, int cols,
                    const std::vector<double>& values) {
    require(matrix.row_s() == rows, "Satır sayısı beklenenden farklı");
    require(matrix.col_s() == cols, "Sütun sayısı beklenenden farklı");
    require(values.size() == static_cast<size_t>(matrix.size()), "Test verisi boyutu hatalı");
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            require_close(matrix.at(row, col), values[row * cols + col], "Matris değeri farklı");
        }
    }
}

Matrix sample_matrix() {
    Matrix matrix(2, 3);
    matrix.at(0, 0) = 1.0;
    matrix.at(0, 1) = 2.0;
    matrix.at(0, 2) = 3.0;
    matrix.at(1, 0) = 4.0;
    matrix.at(1, 1) = 5.0;
    matrix.at(1, 2) = 6.0;
    return matrix;
}

void test_construction_and_access() {
    Matrix matrix = sample_matrix();
    require_matrix(matrix, 2, 3, {1, 2, 3, 4, 5, 6});
    require(!matrix.isSquare(), "Dikdörtgen matris kare görünmemeli");

    const Matrix constant = matrix;
    require_close(constant.at(1, 2), 6.0, "const at sonucu farklı");

    bool out_of_range = false;
    try {
        matrix.at(2, 0);
    } catch (const std::out_of_range&) {
        out_of_range = true;
    }
    require(out_of_range, "Geçersiz indeks hata üretmedi");

    bool negative_size = false;
    try {
        Matrix invalid(-1, 2);
    } catch (const std::invalid_argument&) {
        negative_size = true;
    }
    require(negative_size, "Negatif boyut hata üretmedi");

    Matrix copy(matrix);
    Matrix assigned(1, 1);
    assigned = matrix;
    Matrix moved(std::move(copy));
    Matrix move_assigned(1, 1);
    move_assigned = std::move(assigned);
    require_matrix(moved, 2, 3, {1, 2, 3, 4, 5, 6});
    require_matrix(move_assigned, 2, 3, {1, 2, 3, 4, 5, 6});
    require_matrix(matrix.clone(), 2, 3, {1, 2, 3, 4, 5, 6});
}

void test_arithmetic() {
    Matrix left(2, 2);
    left.at(0, 0) = 1;
    left.at(0, 1) = 2;
    left.at(1, 0) = 3;
    left.at(1, 1) = 4;
    Matrix right = Matrix::ones(2, 2);

    require_matrix(left.add(right), 2, 2, {2, 3, 4, 5});
    require_matrix(left.subtract(right), 2, 2, {0, 1, 2, 3});
    require_matrix(left.elementwiseMultiply(right), 2, 2, {1, 2, 3, 4});
    require_matrix(left.multiplyScalar(2.0), 2, 2, {2, 4, 6, 8});
    require_matrix(left.multiply(right), 2, 2, {3, 3, 7, 7});

    bool mismatch = false;
    try {
        left.add(Matrix(1, 2));
    } catch (const std::invalid_argument&) {
        mismatch = true;
    }
    require(mismatch, "Boyut uyuşmazlığı hata üretmedi");
}

void test_transforms_and_statistics() {
    require_matrix(Matrix::identity(3), 3, 3, {1, 0, 0, 0, 1, 0, 0, 0, 1});
    require_matrix(Matrix::zeros(2, 2), 2, 2, {0, 0, 0, 0});
    require_matrix(Matrix::ones(2, 3), 2, 3, {1, 1, 1, 1, 1, 1});

    Matrix matrix = sample_matrix();
    require_close(matrix.mean(), 3.5, "Ortalama farklı");
    require_close(matrix.l1_norm(), 21.0, "L1 normu farklı");
    require_close(matrix.l2_norm(), std::sqrt(91.0), "L2 normu farklı");
    matrix.negate();
    require_matrix(matrix, 2, 3, {-1, -2, -3, -4, -5, -6});

    Matrix square(2, 2);
    square.at(0, 0) = 2;
    square.at(0, 1) = 1;
    square.at(1, 0) = 4;
    square.at(1, 1) = 3;
    require_close(square.trace(), 5.0, "Trace farklı");

    Matrix flattened = sample_matrix();
    flattened.flatten();
    require_matrix(flattened, 1, 6, {1, 2, 3, 4, 5, 6});

    require_matrix(sample_matrix().transpose(), 3, 2, {1, 4, 2, 5, 3, 6});

    Matrix randomized(4, 4);
    randomized.randomize(-0.25, 0.25);
    for (int row = 0; row < randomized.row_s(); ++row) {
        for (int col = 0; col < randomized.col_s(); ++col) {
            require(randomized.at(row, col) >= -0.25 && randomized.at(row, col) <= 0.25,
                    "Rastgele deger aralik disinda");
        }
    }
}

void test_activation() {
    Matrix matrix(2, 2);
    matrix.at(0, 0) = 1;
    matrix.at(0, 1) = 2;
    matrix.at(1, 0) = 1000;
    matrix.at(1, 1) = 1000;
    Matrix copy = matrix.NewSoftMax();
    matrix.softmax();

        require(std::abs(matrix.at(0, 0) + matrix.at(0, 1) - 1.0) < 1e-6,
            "Softmax satırı normalize değil");
        require(std::abs(matrix.at(1, 0) + matrix.at(1, 1) - 1.0) < 1e-6,
            "Softmax taşma güvenliği bozuk");
    require_close(copy.at(0, 0), matrix.at(0, 0), "NewSoftMax sonucu farklı");

    Matrix relu_matrix(1, 3);
    relu_matrix.at(0, 0) = -2.0;
    relu_matrix.at(0, 1) = 0.0;
    relu_matrix.at(0, 2) = 3.0;
    relu_matrix.relu();
    require_matrix(relu_matrix, 1, 3, {0, 0, 3});
}

void test_slice_and_merge() {
    Matrix matrix = sample_matrix();
    require_matrix(matrix.slice(0, 2, 1, 3), 2, 2, {2, 3, 5, 6});

    Matrix top(1, 2);
    top.at(0, 0) = 1;
    top.at(0, 1) = 2;
    Matrix bottom(1, 2);
    bottom.at(0, 0) = 3;
    bottom.at(0, 1) = 4;
    require_matrix(top.concatVertical(bottom), 2, 2, {1, 2, 3, 4});
    require_matrix(top.concatHorizontal(bottom), 1, 4, {1, 2, 3, 4});

    const std::vector<const Matrix*> matrices{&top, &bottom};
    require_matrix(Matrix::concatenate(matrices, 0), 2, 2, {1, 2, 3, 4});
    require_matrix(Matrix::concatenate(matrices, 1), 1, 4, {1, 2, 3, 4});
}

void test_advanced_math() {
    Matrix matrix(2, 2);
    matrix.at(0, 0) = 4;
    matrix.at(0, 1) = 7;
    matrix.at(1, 0) = 2;
    matrix.at(1, 1) = 6;
    require_close(matrix.determinant(), 10.0, "Determinant farklı");
    require_matrix(matrix.Inverse(), 2, 2, {0.6, -0.7, -0.2, 0.4});

    Matrix diagonal(2, 2);
    diagonal.at(0, 0) = 4;
    diagonal.at(1, 1) = 2;
    LUDecomposition lu = diagonal.lu_decompose();
    require_matrix(lu.L, 2, 2, {1, 0, 0, 1});
    require_matrix(lu.U, 2, 2, {4, 0, 0, 2});
    require_matrix(lu.P.multiply(diagonal), 2, 2, {4, 0, 0, 2});

    Matrix eigen(2, 2);
    eigen.at(0, 0) = 2;
    eigen.at(1, 1) = 1;
    auto result = eigen.power_iteration(1000, 1e-8);
    require_close(result.first, 2.0, "Power iteration eigenvalue sonucu farklı");

    Matrix pivoted(2, 2);
    pivoted.at(0, 0) = 0;
    pivoted.at(0, 1) = 2;
    pivoted.at(1, 0) = 1;
    pivoted.at(1, 1) = 3;
    LUDecomposition pivoted_lu = pivoted.lu_decompose();
    require_matrix(pivoted_lu.P.multiply(pivoted), 2, 2, {1, 3, 0, 2});
    require_matrix(pivoted_lu.L.multiply(pivoted_lu.U), 2, 2, {1, 3, 0, 2});

    Matrix singular(2, 2);
    singular.at(0, 0) = 1;
    singular.at(0, 1) = 2;
    singular.at(1, 0) = 2;
    singular.at(1, 1) = 4;
    bool lu_failed = false;
    try {
        singular.lu_decompose();
    } catch (const std::runtime_error&) {
        lu_failed = true;
    }
    require(lu_failed, "Tekil LU ayrıştırması hata üretmedi");

    bool inverse_failed = false;
    try {
        singular.Inverse();
    } catch (const std::runtime_error&) {
        inverse_failed = true;
    }
    require(inverse_failed, "Tekil matris tersi hata üretmedi");
}

void test_broadcasting() {
    Matrix matrix = sample_matrix();
    Matrix row(1, 3);
    row.at(0, 0) = 10;
    row.at(0, 1) = 20;
    row.at(0, 2) = 30;
    require_matrix(matrix.addBroadcast(row), 2, 3, {11, 22, 33, 14, 25, 36});
    require_matrix(matrix.multiplyBroadcast(row), 2, 3, {10, 40, 90, 40, 100, 180});
}

void test_serialization_and_print() {
    const std::string filename = "matrix_test.bin";
    Matrix original = sample_matrix();
    original.saveBinary(filename);
    Matrix loaded = Matrix::loadBinary(filename);
    require_matrix(loaded, 2, 3, {1, 2, 3, 4, 5, 6});
    std::remove(filename.c_str());

    const std::string truncated_filename = "matrix_truncated.bin";
    {
        std::ofstream file(truncated_filename, std::ios::binary);
        const int rows = 2;
        const int cols = 2;
        const double value = 1.0;
        file.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
        file.write(reinterpret_cast<const char*>(&cols), sizeof(cols));
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }
    bool truncated_failed = false;
    try {
        Matrix::loadBinary(truncated_filename);
    } catch (const std::runtime_error&) {
        truncated_failed = true;
    }
    std::remove(truncated_filename.c_str());
    require(truncated_failed, "Eksik binary veri hata üretmedi");

    const std::string invalid_header_filename = "matrix_invalid_header.bin";
    {
        std::ofstream file(invalid_header_filename, std::ios::binary);
        const int rows = -1;
        const int cols = 2;
        file.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
        file.write(reinterpret_cast<const char*>(&cols), sizeof(cols));
    }
    bool invalid_header_failed = false;
    try {
        Matrix::loadBinary(invalid_header_filename);
    } catch (const std::runtime_error&) {
        invalid_header_failed = true;
    }
    std::remove(invalid_header_filename.c_str());
    require(invalid_header_failed, "Gecersiz binary boyutu hata üretmedi");

    Console console;
    original.print(console);
}

} // namespace

int main() {
    const std::vector<std::pair<std::string, std::function<void()>>> tests = {
        {"construction_and_access", test_construction_and_access},
        {"arithmetic", test_arithmetic},
        {"transforms_and_statistics", test_transforms_and_statistics},
        {"activation", test_activation},
        {"slice_and_merge", test_slice_and_merge},
        {"advanced_math", test_advanced_math},
        {"broadcasting", test_broadcasting},
        {"serialization_and_print", test_serialization_and_print}
    };

    for (const auto& [name, test] : tests) {
        try {
            test();
            std::cout << "[PASS] " << name << '\n';
        } catch (const std::exception& error) {
            std::cerr << "[FAIL] " << name << ": " << error.what() << '\n';
            return 1;
        }
    }

    std::cout << "Tum Matrix testleri basarili.\n";
    return 0;
}
