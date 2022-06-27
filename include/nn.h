#ifndef NN_H
#define NN_H

#include "matrix.h"
#include "img.h"

class NeuralNetwork 
{
public:
    NeuralNetwork(int input, int hidden, int output, double lr);
    NeuralNetwork(std::string file_string);
    ~NeuralNetwork() {};

    void train(Matrix input_data, Matrix output_data);
    void train_batch_imgs(std::vector<Img> imgs, int batch_size);
    Matrix predict_img(Img img);
    double predict_imgs(std::vector<Img> imgs, int n);
    Matrix predict(Matrix input_data);
    void save(std::string file_string);
    void print();

	int m_input;
	int m_hidden;
	int m_output;
	double m_learning_rate;
	Matrix m_hidden_weights = Matrix(10, 10);
	Matrix m_output_weights = Matrix(10, 10);
};

#endif // NN_H