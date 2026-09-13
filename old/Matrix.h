//
// Created by muham on 8.09.2026.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using namespace std;

/*
 * ilk önce matrix kodumuzu oluşturuyoruz burada gördüğünüz gibi matrix adında bir veri tipi oluşturarak
 * işimize başlıyoruz class ile matrix adındaki yeni veri türümüzü tanımlamış oluyoruz
 *
 * prive kısımı sadece matrix kütüpanesinin erişebileceği bir kısımdır bu kısıma fonksyonu çağıran dosya erişemez
 * public kısımı ise kütüphaneyi kullanana dosyaya verdiğimiz kısımdır
 */
class Console;

class Matrix {

private:
    int rows;
    int cols;

    std::vector<float> data;
    bool isValidIndex(int row_num, int col_num);

public:
    //Matrix init kısımı
    Matrix(int rows, int cols);// Matrix init
    float& at(int row, int col);// Matrix Değer Ekleme
    void print(Console& console);// Matrix Ekran Yazma
    double mean();
    void negate();

    //Matrix Details
    int row_s() const;
    int col_s() const;
    int size() const;
    bool isSquare() const;


    //Matrix 4 işlem
    Matrix multiply(Matrix& Other_Matrix);// Matrix Çarpma
    Matrix add(Matrix& Other_Matrix);
    Matrix subtract(Matrix& Other_Matrix);
    Matrix elementwiseMultiply(Matrix& Other_Matrix);

    //SoftMax kısımı
    void softmax();
    Matrix NewSoftMax() const;

    //Mevcut Matrix Dönüşümleri
    static Matrix identity(int size);   // Yeni birim matris döndürür
    void setIdentity();                 // Mevcut matrisi birim matris yapar (kare olmalı)
    static Matrix zeros(int rows, int cols);  // Yeni sıfır matris döndürür
    void zero();                        // Mevcut matrisi sıfırlar
    void flatten(); // elimizdeki matrixsi tek boyutlu 1D yapma

    // kopyalama fonksyonları
    Matrix(const Matrix& other);                  // Copy Constructor
    Matrix& operator=(const Matrix& other);       // Copy Assignment
    Matrix clone() const;

    //Matrix normları
    double l1_norm() const;
    double l2_norm() const;

    //Diğer Matrix operatörleri
    double trace();



};

#endif //MATRIX_H
