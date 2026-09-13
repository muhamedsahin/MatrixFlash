#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <string>
#include <utility>

class Console;
struct LUDecomposition;

class Matrix {
private:
    int rows;
    int cols;
    std::vector<double> data;

    bool isValidIndex(int row_num, int col_num) const;

    // Ortak LU çekirdeği: lu_decompose() ile Inverse() aynı LU mantığını
    // paylaşır. false dönerse matris tekil (singular) demektir;
    // LU (birleşik L+U) ve pivot sonucu referans parametrelere yazılır.
    static bool luDecomposeCore(const std::vector<double>& src, int n,
                                std::vector<double>& LU, std::vector<int>& pivot);

public:
    // ==========================================
    // 1. YAPICILAR (CONSTRUCTORS)
    // ==========================================
    Matrix(int rows, int cols);

    // Kopyalama (Mevcut)
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    Matrix clone() const;

    // TAŞIMA (MOVE) - PRO SEVİYE PERFORMANS İÇİN ŞART!
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(Matrix&& other) noexcept;

    // ==========================================
    // 2. ERİŞİM VE BİLGİ
    // ==========================================
    double& at(int row, int col);
    const double& at(int row, int col) const;

    int row_s() const;
    int col_s() const;
    int size() const;
    bool isSquare() const;
    void print(Console& console) const;

    // ==========================================
    // 3. TEMEL İŞLEMLER (4 İŞLEM)
    // ==========================================
    Matrix multiply(const Matrix& Other_Matrix) const;
    Matrix add(const Matrix& Other_Matrix) const;
    Matrix subtract(const Matrix& Other_Matrix) const;
    Matrix elementwiseMultiply(const Matrix& Other_Matrix) const;

    // ==========================================
    // 4. İLERİ DÜZEY MATEMATİK
    // ==========================================
    Matrix Inverse() const;
    double determinant() const;
    double trace() const;
    double l1_norm() const;
    double l2_norm() const;
    double mean() const;
    void negate();

    // ==========================================
    // 5. YAPAY ZEKA / DÖNÜŞÜMLER
    // ==========================================
    void softmax();
    Matrix NewSoftMax() const;
    void flatten();

    // ==========================================
    // 6. MATRİS OLUŞTURUCULAR (FACTORY)
    // ==========================================
    static Matrix identity(int size);
    static Matrix ones(int rows, int cols = 1);
    static Matrix zeros(int rows, int cols);
    void setIdentity();
    void zero();

    // ==========================================
    // 7. DİLİMLEME VE BİRLEŞTİRME (SLICE & CONCAT)
    // ==========================================
    Matrix slice(int rowStart, int rowEnd, int colStart, int colEnd) const;
    Matrix concatVertical(const Matrix& other) const;
    Matrix concatHorizontal(const Matrix& other) const;

    // KRİTİK DÜZELTME: static yapıldı
    static Matrix concatenate(const std::vector<const Matrix*>& matrices, int axis);

    // ==========================================
    // YENİ: İLERİ DÜZEY AYRIŞTIRMA (DECOMPOSITION)
    // ==========================================
    LUDecomposition lu_decompose() const;
    // Not: QR decomposition için Householder veya Gram-Schmidt gerekir.
    // From-scratch için çok uzundur, gerekirse Eigen kütüphanesi entegre edilir.

    // ==========================================
    // YENİ: EİGENVALUE / EİGENVECTOR (PCA için)
    // ==========================================
    // En baskın (dominant) eigenvalue ve eigenvector'ü bulur (Power Iteration)
    std::pair<double, Matrix> power_iteration(int max_iters = 1000, double tolerance = 1e-6) const;

    // ==========================================
    // YENİ: BROADCASTING (NumPy tarzı)
    // ==========================================
    // Örnek: 3x3 Matris + 1x3 Vektör -> Vektör tüm satırlara yayılır
    Matrix addBroadcast(const Matrix& other) const;
    Matrix multiplyBroadcast(const Matrix& other) const;

    // ==========================================
    // YENİ: SERİALİZASYON (Dosyaya Kaydet/Yükle)
    // ==========================================
    void saveBinary(const std::string& filename) const;
    static Matrix loadBinary(const std::string& filename);
};

struct LUDecomposition {
    Matrix L;
    Matrix U;
    Matrix P;
};

#endif // MATRIX_HPP