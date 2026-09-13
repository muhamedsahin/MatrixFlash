//
// Created by muham on 11.09.2026.
//
//
// Created by muham on 8.09.2026.
//
#include <iostream>
#include "Matrix/Matrix.hpp"
#include "../../Console/obj_console.h"

using namespace std;

int main() {

    Console console;

    Matrix A(2, 2);
    A.at(0,0) = 1; A.at(0,1) = 2;
    A.at(1,0) = 3; A.at(1,1) = 4;

    Matrix B(2, 2);
    B.at(0,0) = 10; B.at(0,1) = 20;
    B.at(1,0) = 30; B.at(1,1) = 40;

    Matrix C = A.add(B);
    C.print(console);
    C.softmax();
    C.print(console);

    console.log("Matrix Ortalamasi degerleri icin",A.mean());


    return 0;
}