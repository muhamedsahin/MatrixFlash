#include <iostream>
#include <numeric>
#include <cmath>
#include <execution>
#include <stdexcept>
#include "./Matrix.h"
#include "../Console/obj_console.h"

/*
 * Bu kısımda matrix C dosyamızın içindeki matrix class veri türümüzün içindeki fuction çağırıyoruz
 * Bu kısımda çok fazla matrix adı verdiğimizi için karmaşık geliyor şimdi tanımlardığımız class
 * belirli bir fonksyondan değer alıyor bu aldığı değer kendi claasın içindeki prive kısımdaki ne göre
 * bir çıktı üretme imkanna sahip oluyor şimdi gereklği özel çıktı üretebilen fonkyonumuzu matrix classının
 * içindeki matrix fonksyonu ile tanımladık :: buna scope operatör deniyor yani claasın içindeki fonkyona
 * erişmemizi dağlıyor ve bu foknyon rows ve cols gibi değerler alıyor gerekli matrix nesnemizi oluşturmak için
 * biz bu kısımda this kullanarak matrix classınını içindeki rows ve cols değişkenlerine bu değeri gönderiyoruz
 * this kısımını kullanmak zorunda değiliz fakat aynı isimi birden fazla defa kullandığımız için isim çakışması
 * olmaması için thisi kullanarak 1. rowsun classa ait olduğunu 2. rowsun ise nesneyi oluşturan fonkyona verilen
 * girdi olduğunu anlamak için kullanıyoruz
 */

Matrix::Matrix(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;

    data.resize(rows * cols);
}

bool Matrix::isValidIndex(int row_num, int col_num) {
    // ✅ DOĞRUSU: row_num < rows ve col_num < cols olmalı
    if (row_num >= 0 && row_num < rows && col_num >= 0 && col_num < cols) {
        return true;
    } else {
        return false;
    }
}

/*
 * bu fonksyon sanılan klasik fonksyonun aksine return ettiği değer bir çıktı değildir
 * float olarak verdiğimiz girdi değeri return ettiğimiz değişkene atanır
 * rows ve cols lar 0 dan başlar böyle hayal edersen 2 boyutlu bir matrisin tek
 * boyutlu bir vektördeki her hangi bir noktasının konumunu bulabilirsin.
 */
float &Matrix::at(int row, int col) {
    if (isValidIndex(row, col)) {
        return this->data[row * cols + col];
    } else {
        throw std::out_of_range("Girilen Col ve Row kısımları oluşturulan matris ile uyuşmamaktadır");
    }
}

void Matrix::print(Console &console) {
    for (int i = 0; i < rows; i++) {
        console.print("[");
        for (int j = 0; j < cols; j++) {
            console.print(data[i * cols + j],(j < cols - 1)? "," : "");
        }
        console.log("]");
    }
}

void Matrix::negate() {
    std::transform(std::execution::par_unseq, data.begin(), data.end(), data.begin(), [](float x) { return -x; });
}

double Matrix::mean() {
    double sum = std::reduce(std::execution::par_unseq, data.begin(), data.end(), 0.0);
    return sum/data.size();
}

/*
 * Const referans aldığımız değişken üzerinde değişiklik yapmıyacağımızı gösterir
 */
Matrix Matrix::multiply(Matrix &Other_Matrix) {
    if (this -> cols != Other_Matrix.rows) {
        throw std::invalid_argument("Matris çarpımı için iç boyutlar uyuşmuyor!");
    }

    int New_Matrix_Col = Other_Matrix.cols;
    int New_Matrix_Row = this -> rows;

    Matrix NewMatrix = Matrix(New_Matrix_Row, New_Matrix_Col);

    /*
     * Şimdi sırasıyla yeni matriximizin her satırını doldurarak başlıyoruz
     * Daha sonrasında o satırdaki büttün kolonları dolduruyoruz
     */
    for (int i = 0; i < New_Matrix_Row; i++) {
        for (int j = 0; j < New_Matrix_Col; j++) {

            float sum = 0; // Her hücre için başlangıç değeri

            for (int k = 0; k < this -> cols; k++) {
                sum += this -> at(i, k) * Other_Matrix.at(k, j);
            }

            NewMatrix.at(i, j) = sum;
        }
    }

    return NewMatrix;
}

Matrix Matrix::add(Matrix &Other_Matrix) {
    if (this->rows != Other_Matrix.rows || this->cols != Other_Matrix.cols) {
        throw std::invalid_argument("Matris toplama için boyutlar tam olarak eşit olmalı!");
    }

    Matrix NewMatrix(this->rows, this->cols);

    for (size_t i = 0; i < this->data.size(); i++) {
        NewMatrix.data[i] = this->data[i] + Other_Matrix.data[i];
    }

    return NewMatrix;
}

Matrix Matrix::subtract(Matrix& Other_Matrix) {
    if (this->rows != Other_Matrix.rows || this->cols != Other_Matrix.cols) {
        throw std::invalid_argument("Matris toplama için boyutlar tam olarak eşit olmalı!");
    }

    Matrix NewMatrix(this->rows, this->cols);

    for (size_t i = 0; i < this->data.size(); i++) {
        NewMatrix.data[i] = this->data[i] - Other_Matrix.data[i];
    }
    return NewMatrix;
}

Matrix Matrix::elementwiseMultiply(Matrix& Other_Matrix) {
    if (this -> rows != Other_Matrix.rows || this -> cols != Other_Matrix.cols) {
        throw std::invalid_argument("Matris elementel çarpım için boyutlar tam olarak eşit olmalı!");
    }

    Matrix New_Matrix(this->rows, this->cols);

    for (size_t i = 0; i < this -> data.size(); i++) {
        New_Matrix.data[i] = this-> data[i] * Other_Matrix.data[i];
    }

    return New_Matrix;
}

void Matrix::softmax() {
    // Her satırı paralel olarak işle
#pragma omp parallel for schedule(static)
    for (int i = 0; i < rows; ++i) {

        auto row_begin = data.begin() + i * cols;
        auto row_end   = row_begin + cols;

        // 1. Satırın maksimumunu bul
        float max_val = *std::max_element(std::execution::par_unseq, row_begin, row_end);

        // 2. exp(x - max) hesapla + toplamı al
        float sum = std::transform_reduce(
            std::execution::par_unseq,
            row_begin, row_end,
            0.0f,
            std::plus<>(),
            [max_val](float x) {
                return std::exp(x - max_val);
            }
        );

        // 3. Normalize et (direkt orijinal data'nın üzerine yaz)
        const float inv_sum = 1.0f / sum;

        std::transform(
            std::execution::par_unseq,
            row_begin, row_end,
            row_begin,                    // ← aynı yere yazıyoruz
            [max_val, inv_sum](float x) {
                return std::exp(x - max_val) * inv_sum;
            }
        );
    }
}

Matrix Matrix::NewSoftMax() const {
    Matrix result(rows, cols);

    // Her satırı paralel olarak işle
    #pragma omp parallel for schedule(static)   // OpenMP varsa bunu kullan (daha hızlı olur)
    // Eğer OpenMP istemiyorsan aşağıdaki for'u normal bırak + içeride par_unseq kullan
    for (int i = 0; i < rows; ++i) {

        // Bu satırın başlangıç ve bitiş iterator'ları
        auto row_begin = data.begin() + i * cols;
        auto row_end   = row_begin + cols;

        auto result_begin = result.data.begin() + i * cols;

        // 1. Satırın maksimumunu bul (stabilite için)
        float max_val = *std::max_element(std::execution::par_unseq, row_begin, row_end);

        // 2. exp(x - max) hesapla + toplamı al
        float sum = std::transform_reduce(
            std::execution::par_unseq,
            row_begin, row_end,
            0.0f,
            std::plus<>(),
            [max_val](float x) {
                return std::exp(x - max_val);
            }
        );

        // 3. Normalize et
        const float inv_sum = 1.0f / sum;

        std::transform(
            std::execution::par_unseq,
            row_begin, row_end,
            result_begin,
            [max_val, inv_sum](float x) {
                return std::exp(x - max_val) * inv_sum;
            }
        );
    }

    return result;
}


Matrix Matrix::identity(int size) {
    Matrix m(size, size);
    m.setIdentity();
    return m;
}

void Matrix::setIdentity() {
    if (rows != cols) {
        throw std::invalid_argument("Birim matris sadece kare matrislerde oluşturulabilir!");
    }

    // Önce her şeyi sıfırla
    std::fill(std::execution::par_unseq, data.begin(), data.end(), 0.0f);

    // Sonra köşegenleri 1 yap
    for (int i = 0; i < rows; ++i) {
        data[i * cols + i] = 1.0f;
    }
}

void Matrix::zero() {
    std::fill(std::execution::par_unseq, data.begin(), data.end(), 0.0f);
}

Matrix Matrix::zeros(int rows, int cols) {
    Matrix m(rows, cols);
    m.zero();
    return m;
}

Matrix::Matrix(const Matrix& other) {
    rows = other.rows;
    cols = other.cols;
    data = other.data;          // vector kendi içinde derin kopya yapar
}

// Copy Assignment Operator
Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {       // Kendi kendine atamayı engelle
        return *this;
    }

    rows = other.rows;
    cols = other.cols;
    data = other.data;          // yine derin kopya

    return *this;
}

// Kolay kullanım için clone
Matrix Matrix::clone() const {
    return Matrix(*this);       // Copy constructor'ı çağırır
}


/*
 * Matrix Norm bölümü start
 */

//bütün elemanları topla
double Matrix::l1_norm() const {

    return std::transform_reduce(
        std::execution::par_unseq,
        data.begin(),
        data.end(),
        0.0,
        std::plus<>(),
        [](float x) { return std::abs(x); }
    );
}

// Karelerini alt topla kare kökünü al
double Matrix::l2_norm() const {
    double sum_sq = std::transform_reduce(
        std::execution::par_unseq,
        data.begin(),
        data.end(),
        0.0,
        std::plus<>(),
        [](float x) { return static_cast<double>(x) * x; }
    );
    return std::sqrt(sum_sq);
}

/*
 * Matrix Flatten
 */
void Matrix::flatten() {
    this -> cols = rows * cols;
    this -> rows = 1;
}

/*
 * Matrix Details
 */

int Matrix::row_s() const {
    return this -> rows;
}
int Matrix::col_s() const {
    return this -> cols;
}
int Matrix::size() const {
    return this -> rows * this -> cols;
}
bool Matrix::isSquare() const {
    return (this -> rows == this -> cols);
}



/*
 * Matrix Diğer
 */

double Matrix::trace() {
    if (this-> rows != this -> cols) {
        throw std::invalid_argument("Matrix en boy boyutları birbirine eşit olması lazım");
    }

    double sum = 0.0;
    for (int i = 0; i < this -> rows; i++) {
        sum += data[i*(rows + 1)];
    }
    return sum;
}




































