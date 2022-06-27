#include "matrix.h"
#include <math.h>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>

#define MAXCHAR 100

Matrix::Matrix(uint32_t rows, uint32_t columns) 
	: m_rows(rows), m_columns(columns), m_entries(rows, std::vector<double>(columns, 0.0))
{
}

Matrix::Matrix(const Matrix& in_mat) 
	: m_rows(in_mat.m_rows), m_columns(in_mat.m_columns), m_entries(in_mat.m_entries)
{
}

Matrix::Matrix(std::string file_string) 
{
	FILE* file = fopen(file_string.c_str(), "r");

	char entry[MAXCHAR]; 
	std::fgets(entry, MAXCHAR, file);
	int rows = std::atoi(entry);
	std::fgets(entry, MAXCHAR, file);
	int cols = std::atoi(entry);

	*this = Matrix(rows, cols);

	for (int i = 0; i < m_rows; i++) 
    {
		for (int j = 0; j < m_columns; j++) 
        {
			std::fgets(entry, MAXCHAR, file);
			m_entries[i][j] = std::strtod(entry, NULL);
		}
	}

	std::cout << "Sucessfully loaded matrix from " << file_string << std::endl;
	fclose(file);
}

std::ostream &operator<<(std::ostream &os, Matrix const &in_mat) { 
	return os << '(' << in_mat.m_columns << ", " << in_mat.m_rows << ')';
}

void Matrix::fill(int16_t n) 
{
	for (int i = 0; i < m_rows; i++) 
    {
		for (int j = 0; j < m_columns; j++) 
        {
			m_entries[i][j] = n;
		}
	}
}

void Matrix::print() 
{
	std::cout << "Rows: " << m_rows << " Columns: " << m_columns << std::endl;
}

void Matrix::save(std::string file_string) 
{
	FILE* file = fopen(file_string.c_str(), "w");

	fprintf(file, "%d\n", m_rows);
	fprintf(file, "%d\n", m_columns);

	for (int i = 0; i < m_rows; i++) 
    {
		for (int j = 0; j < m_columns; j++) 
        {
			fprintf(file, "%.6f\n", m_entries[i][j]);
		}
	}

	std::cout << "Successfully saved matrix to " << file_string << std::endl;
	fclose(file);
}

double uniform_distribution(double low, double high) 
{
	double difference = high - low; // The difference between the two
	int scale = 10000;
	int scaled_difference = (int)(difference * scale);
	return low + (1.0 * (rand() % scaled_difference) / scale);
}

void Matrix::randomize(uint16_t n) 
{
	// Pulling from a random distribution of 
	// Min: -1 / sqrt(n)
	// Max: 1 / sqrt(n)
	double min = -1.0 / sqrt(n);
	double max = 1.0 / sqrt(n);
	for (int i = 0; i < m_rows; i++) 
    {
		for (int j = 0; j < m_columns; j++) 
        {
			m_entries[i][j] = uniform_distribution(min, max);
		}
	}
}

uint32_t Matrix::arg_max() 
{
	// Expects a Mx1 matrix
	double max_score = 0;
	uint32_t max_idx = 0;

	for (int i = 0; i < m_rows; i++) 
    {
		if (m_entries[i][0] > max_score) 
        {
			max_score = m_entries[i][0];
			max_idx = i;
		}
	}

	return max_idx;
}

void Matrix::flatten(bool axis) 
{
	// Axis = 0 -> Column Vector, Axis = 1 -> Row Vector
    // int rows =    (((m_rows * m_columns) - 1) * !axis) + 1;
    // int columns = (((m_rows * m_columns) - 1) * axis) + 1;
	int rows = 1, columns = 1;

	if (axis == false) 
		rows = m_rows * m_columns;
	else
		columns = m_rows * m_columns;

    Matrix mat = Matrix(rows, columns);
    
	for (int i = 0; i < m_rows; i++) 
    {
		for (int j = 0; j < m_columns; j++) 
        {
			if (axis == false) mat.m_entries[i * m_columns + j][0] = m_entries[i][j];
			else      mat.m_entries[0][i * m_columns + j] = m_entries[i][j];
		}
	}
    
    *this = mat;
}

bool Matrix::check_dimensions(const Matrix& mat) 
{
	return m_rows == mat.m_rows && m_columns == mat.m_columns;
}

Matrix Matrix::multiply(const Matrix& in_mat) 
{
	if (check_dimensions(in_mat)) 
    {
		Matrix mat = Matrix(in_mat);

		for (int i = 0; i < mat.m_rows; i++) 
        {
			for (int j = 0; j < in_mat.m_columns; j++) 
            {
				mat.m_entries[i][j] = m_entries[i][j] * in_mat.m_entries[i][j];
			}
		}

		return mat;
	} 
    else 
    {
		std::cout << "Dimension mistmatch multiply: " << *this << " & " << in_mat << std::endl;
		exit(1);
	}
}

Matrix Matrix::add(const Matrix& in_mat) 
{
	if (check_dimensions(in_mat)) 
    {
		Matrix mat = Matrix(m_rows, m_columns);

		for (int i = 0; i < m_rows; i++) 
        {
			for (int j = 0; j < m_columns; j++) 
            {
				mat.m_entries[i][j] = m_entries[i][j] + in_mat.m_entries[i][j];
			}
		}

		return mat;
	} 
    else 
    {
		std::cout << "Dimension mistmatch add: " << *this << " & " << in_mat  << std::endl;
		// printf("Dimension mistmatch add: %dx%d %dx%d\n", m_rows, m_columns, mat.m_rows, mat.m_columns);
		exit(1);
	}
}

Matrix Matrix::subtract(const Matrix& in_mat) 
{
	if (check_dimensions(in_mat)) 
    {
		Matrix mat = Matrix(m_rows, m_columns);

		for (int i = 0; i < m_rows; i++) 
        {
			for (int j = 0; j < in_mat.m_columns; j++) 
            {
				mat.m_entries[i][j] = m_entries[i][j] - in_mat.m_entries[i][j];
			}
		}

		return mat;
	} 
    else 
    {
		std::cout << "Dimension mistmatch subtract: " << *this << " & " << in_mat << std::endl;
		// printf("Dimension mistmatch subtract: %dx%d %dx%d\n", m_rows, m_columns, mat.m_rows, mat.m_columns);
		exit(1);
	}
}

Matrix Matrix::apply(function_t func) 
{
	for (int i = 0; i < m_rows; i++) 
    {
		for (int j = 0; j < m_columns; j++) 
        {
			m_entries[i][j] = func(m_entries[i][j]);
		}
	}

	return *this;
}

Matrix Matrix::dot(const Matrix& in_mat) 
{ 
	// m_columns == in_mat.m_rows && m_rows == in_mat.m_columns
	if (m_columns == in_mat.m_rows)
    {
		Matrix mat = Matrix(m_rows, in_mat.m_columns);

		for (int i = 0; i < m_rows; i++) 
        {
			for (int j = 0; j < in_mat.m_columns; j++) 
            {
				double sum = 0;

				for (int k = 0; k < in_mat.m_rows; k++) 
                {
					sum += m_entries[i][k] * in_mat.m_entries[k][j];
				}

				mat.m_entries[i][j] = sum;
			}
		}
		
		return mat;
	} 
    else 
    {
		std::cout << "Dimension mistmatch dot: " << *this << " & " << in_mat << std::endl;
		exit(1);
	}
}

Matrix Matrix::scale(double n) 
{
	Matrix mat = Matrix(*this);

	for (int i = 0; i < m_rows; i++) 
    {
		for (int j = 0; j < m_columns; j++) 
        {
			mat.m_entries[i][j] *= n;
		}
	}

	return mat;
}

Matrix Matrix::add_scalar(double n) 
{
	Matrix mat = Matrix(*this);

	for (int i = 0; i < m_rows; i++) 
    {
		for (int j = 0; j < m_columns; j++) 
        {
			mat.m_entries[i][j] += n;
		}
	}

	return mat;
}

Matrix Matrix::transpose() 
{
	Matrix mat = Matrix(m_columns, m_rows);

	for (int i = 0; i < m_rows; i++) 
    {
		for (int j = 0; j < m_columns; j++) 
        {
			mat.m_entries[j][i] = m_entries[i][j];
		}
	}
	return mat;
}

double sigmoid(double input) 
{
	return 1.0 / (1 + exp(-1 * input));
}

Matrix Matrix::sigmoid_prime() 
{
	Matrix ones = Matrix(m_rows, m_columns);

	ones.fill(1);

	Matrix subtracted = subtract(ones);
	Matrix multiplied = multiply(subtracted);

	return multiplied;
}

Matrix Matrix::soft_max() 
{
	double total = 0;

	for (int i = 0; i < m_rows; i++) 
	{
		for (int j = 0; j < m_columns; j++) 
		{
			total += exp(m_entries[i][j]);
		}
	}

	Matrix mat = Matrix(m_rows, m_columns);

	for (int i = 0; i < mat.m_rows; i++) 
	{
		for (int j = 0; j < mat.m_columns; j++) 
		{
			mat.m_entries[i][j] = exp(m_entries[i][j]) / total;
		}
	}

	return mat;
}