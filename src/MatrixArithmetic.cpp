#include "Matrix/Matrix.hpp"
#include <stdexcept>

Matrix Matrix::multiply(const Matrix& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Matris çarpımı için iç boyutlar uyuşmuyor!");
    }

    // Boyut kısaltmaları: A[m x k] * B[k x n] = C[m x n]
    const int m = rows;
    const int k = cols;
    const int n = other.cols;

    // at() içindeki sınır kontrolü bu sıcak iç döngüde büyük maliyettir.
    // Burada yalnızca iç erişim için ham pointer'lar kullanılır.
    const double* A = data.data();
    const double* B = other.data.data();

    Matrix result(m, n); // constructor veriyi 0 ile doldurur
    double* C = result.data.data();

    // =========================================================
    // CACHE-OPTIMIZED i-k-j MATRİS ÇARPIMI
    // ---------------------------------------------------------
    // Klasik i-j-k düzeninde B'nin SÜTUNUNA (B[idx][col]) stride n
    // ile erişilir -> her adım cache-miss üretir.
    // i-k-j düzeninde B'nin k. satırı ile C'nin i. satırı row-major
    // ARDIŞIK okunur; iç döngü FMA (c += aik * b) şeklinde derlenir
    // ve otomatik vektörleşmeye (SIMD) çok uygundur.
    // =========================================================
    for (int i = 0; i < m; ++i) {
        const double* a_row = A + (size_t)i * k;
        double* c_row = C + (size_t)i * n;
        for (int kk = 0; kk < k; ++kk) {
            const double aik = a_row[kk]; // dışta sabit skaler
            const double* b_row = B + (size_t)kk * n;
            for (int j = 0; j < n; ++j) {
                c_row[j] += aik * b_row[j]; // FMA
            }
        }
    }
    return result;
}

Matrix Matrix::add(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matris toplama için boyutlar tam olarak eşit olmalı!");
    }

    Matrix result(rows, cols);
    double* dst = result.data.data();
    const double* a = data.data();
    const double* b = other.data.data();
    const size_t count = data.size();
    // Ardışık bellek erişimi; otomatik vektörleşir
    for (size_t i = 0; i < count; ++i) {
        dst[i] = a[i] + b[i];
    }
    return result;
}

Matrix Matrix::subtract(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matris çıkarma için boyutlar tam olarak eşit olmalı!");
    }

    Matrix result(rows, cols);
    double* dst = result.data.data();
    const double* a = data.data();
    const double* b = other.data.data();
    const size_t count = data.size();
    for (size_t i = 0; i < count; ++i) {
        dst[i] = a[i] - b[i];
    }
    return result;
}

Matrix Matrix::elementwiseMultiply(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matris elementel çarpım için boyutlar tam olarak eşit olmalı!");
    }

    Matrix result(rows, cols);
    double* dst = result.data.data();
    const double* a = data.data();
    const double* b = other.data.data();
    const size_t count = data.size();
    for (size_t i = 0; i < count; ++i) {
        dst[i] = a[i] * b[i];
    }
    return result;
}