//
// Created by muham on 12.09.2026.
//

#include "Matrix/Matrix.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

std::pair<double, Matrix> Matrix::power_iteration(int max_iters, double tolerance) const {
    if (!isSquare()) {
        throw std::invalid_argument("Hata: Eigenvalue sadece kare matrisler icin!");
    }
    if (rows == 0 || max_iters <= 0 || tolerance <= 0.0) {
        throw std::invalid_argument("Hata: Power iteration parametreleri gecersiz!");
    }

    int n = this->rows;
    const double* A = data.data(); // ham pointer (at() sınır kontrolüne gerek yok)

    // Rastgele bir başlangıç vektörü oluştur (veya hepsi 1 olsun)
    Matrix v = Matrix::ones(n, 1);
    double* vd = v.data.data();

    // Vektörü normalize et (L2 normuna böl) - tek geçişte
    double norm = 0.0;
    for (int i = 0; i < n; ++i) norm += vd[i] * vd[i];
    norm = std::sqrt(norm);
    for (int i = 0; i < n; ++i) vd[i] /= norm;

    // v_new tamponu döngü öncesi tek kez ayrılır (allocation yok!)
    Matrix v_new = Matrix::zeros(n, 1);
    double lambda = 0.0;

    for (int iter = 0; iter < max_iters; ++iter) {
        double* vnd = v_new.data.data();

        // v_new = A * v  (matris-vektör çarpımı, row-major)
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            const double* arow = A + (size_t)i * n;
            for (int j = 0; j < n; ++j) {
                sum += arow[j] * vd[j];
            }
            vnd[i] = sum;
        }

        // Yeni eigenvalue tahmini (Rayleigh quotient)
        // lambda = (v^T * A * v) / (v^T * v)  -> v normalize olduğu için payda 1'dir.
        double new_lambda = 0.0;
        for (int i = 0; i < n; ++i) new_lambda += vd[i] * vnd[i];

        // Vektörü tekrar normalize et (L2 normuna böl, tek geçiş)
        double new_norm = 0.0;
        for (int i = 0; i < n; ++i) new_norm += vnd[i] * vnd[i];
        new_norm = std::sqrt(new_norm);
        if (new_norm <= tolerance) {
            throw std::runtime_error("Hata: Power iteration sifir vektore yaklasti!");
        }
        for (int i = 0; i < n; ++i) vnd[i] /= new_norm;

        // Yakınsama kontrolü (Convergence)
        if (std::abs(new_lambda - lambda) < tolerance) {
            return {new_lambda, std::move(v_new)};
        }
        lambda = new_lambda;

        // v = v_new  (kopyalama yerine O(1) veri takası - çok daha hızlı)
        v.data.swap(v_new.data);
        vd = v.data.data();
    }

    throw std::runtime_error("Hata: Power iteration yakinsamadi!");
}