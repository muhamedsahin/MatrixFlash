//
// Created by muham on 12.09.2026.
//

#include "Matrix/Matrix.hpp"
#include <cmath>
#include <stdexcept>

std::pair<double, Matrix> Matrix::power_iteration(int max_iters, double tolerance) const {
    if (!isSquare()) {
        throw std::invalid_argument("Hata: Eigenvalue sadece kare matrisler icin!");
    }
    if (rows == 0 || max_iters <= 0 || tolerance <= 0.0) {
        throw std::invalid_argument("Hata: Power iteration parametreleri gecersiz!");
    }

    int n = this->rows;
    // Rastgele bir başlangıç vektörü oluştur (veya hepsi 1 olsun)
    Matrix v = Matrix::ones(n, 1);

    // Vektörü normalize et (L2 normuna böl)
    double norm = v.l2_norm();
    for(int i=0; i<n; ++i) v.at(i, 0) /= norm;

    double lambda = 0.0;

    for (int iter = 0; iter < max_iters; ++iter) {
        // v_new = A * v
        Matrix v_new = this->multiply(v);

        // Yeni eigenvalue tahmini (Rayleigh quotient)
        // lambda = (v^T * A * v) / (v^T * v)  -> v normalize olduğu için payda 1'dir.
        double new_lambda = 0.0;
        for(int i=0; i<n; ++i) new_lambda += v.at(i, 0) * v_new.at(i, 0);

        // Vektörü tekrar normalize et
        double new_norm = v_new.l2_norm();
        if (new_norm <= tolerance) {
            throw std::runtime_error("Hata: Power iteration sifir vektore yaklasti!");
        }
        for(int i=0; i<n; ++i) v_new.at(i, 0) /= new_norm;

        // Yakınsama kontrolü (Convergence)
        if (std::abs(new_lambda - lambda) < tolerance) {
            return {new_lambda, v_new};
        }
        lambda = new_lambda;
        v = v_new;
    }

    throw std::runtime_error("Hata: Power iteration yakinsamadi!");
}