#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>
#include <stdexcept>

class Console;

class Matrix {
private:
    int rows;
    int cols;
    std::vector<double> data;
    bool isValidIndex(int row_num, int col_num) const;

public:
    // Matrix init kısımı
    Matrix(int rows, int cols);
    double& at(int row, int col);
    const double& at(int row, int col) const;
    void print(Console& console) const; // print matrisi değiştirmediği için const eklendi
    double mean() const;
    void negate();

    // Matrix Details
    int row_s() const;
    int col_s() const;
    int size() const;
    bool isSquare() const;

    // Matrix 4 işlem
    Matrix multiply(const Matrix& Other_Matrix) const; // const referans düzeltmesi
    Matrix add(const Matrix& Other_Matrix) const;
    Matrix subtract(const Matrix& Other_Matrix) const;
    Matrix elementwiseMultiply(const Matrix& Other_Matrix) const;

    // SoftMax kısımı
    void softmax();
    Matrix NewSoftMax() const;

    // Mevcut Matrix Dönüşümleri
    static Matrix identity(int size);
    void setIdentity();
    static Matrix zeros(int rows, int cols);
    void zero();
    void flatten();

    // kopyalama fonksyonları
    Matrix(const Matrix& other);
    Matrix& operator=(const Matrix& other);
    Matrix clone() const;

    // Matrix normları
    double l1_norm() const;
    double l2_norm() const;

    // Diğer Matrix operatörleri
    double trace() const;

    //Matrix tersi operatörleri
    Matrix Inverse() const;

    //Matrix Determinantı
    double determinant() const;

    //Matrix ayırma ve dikey/yatay birleştirme
    Matrix slice(int rowStart, int rowEnd, int colStart, int colEnd) const;
    Matrix concatVertical(const Matrix& other) const;
    Matrix concatHorizontal(const Matrix& other) const;
    Matrix concatenate(const std::vector<const Matrix*>& matrices, int axis);
};

#endif //MATRIX_HPP