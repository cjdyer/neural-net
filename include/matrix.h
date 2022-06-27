#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <string>
#include <functional>
#include <vector>

using function_t = std::function<double (double)>;

class Matrix 
{
public:
	Matrix(uint32_t rows, uint32_t columns);
    Matrix(const Matrix& in_mat);
    Matrix(std::string file_string);

    friend std::ostream &operator<<(std::ostream &os, Matrix const &in_mat);

    void fill(int16_t n);
    void print();
    void save(std::string file_string);
    void randomize(uint16_t n);
    uint32_t arg_max();
    void flatten(bool axis);

    bool   check_dimensions(const Matrix& in_mat); 
    Matrix multiply(const Matrix& in_mat);
    Matrix add(const Matrix& in_mat);
    Matrix subtract(const Matrix& in_mat);
    Matrix dot(const Matrix& in_mat);
    Matrix apply(function_t func);
    Matrix scale(double n);
    Matrix add_scalar(double n);
    Matrix transpose();

    Matrix sigmoid_prime();
    Matrix soft_max();

    std::vector<std::vector<double>> m_entries;
	uint32_t m_rows;
	uint32_t m_columns;
};

double sigmoid(double input);

#endif // MATRIX_H