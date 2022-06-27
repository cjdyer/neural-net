#include "nn.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#define MAXCHAR 1000

// 784, 300, 10
NeuralNetwork::NeuralNetwork(int input, int hidden, int output, double lr)
{
	m_input = input;
	m_hidden = hidden;
	m_output = output;
	m_learning_rate = lr;

	Matrix m_hidden_layer = Matrix(m_hidden, m_input);
	Matrix m_output_layer = Matrix(m_output, m_hidden);

	m_hidden_layer.randomize(m_hidden);
	m_output_layer.randomize(m_output);

	m_hidden_weights = m_hidden_layer;
	m_output_weights = m_output_layer;
}

NeuralNetwork::NeuralNetwork(std::string file_string)
{
	char entry[MAXCHAR];
	chdir(file_string.c_str());
	FILE *descriptor = fopen("descriptor", "r");

	fgets(entry, MAXCHAR, descriptor);
	m_input = atoi(entry);
	fgets(entry, MAXCHAR, descriptor);
	m_hidden = atoi(entry);
	fgets(entry, MAXCHAR, descriptor);
	m_output = atoi(entry);

	fclose(descriptor);

	m_hidden_weights = Matrix("m_hidden");
	m_output_weights = Matrix("m_output");

	std::cout << "Successfully loaded network from: " << file_string << std::endl;
	chdir("-"); // Go back to the original directory
}

void NeuralNetwork::train(Matrix input, Matrix output)
{
	// Feed forward
	Matrix hidden_inputs = m_hidden_weights.dot(input);
	Matrix hidden_outputs = hidden_inputs.apply(sigmoid);
	Matrix final_inputs = m_output_weights.dot(hidden_outputs);
	Matrix final_outputs = final_inputs.apply(sigmoid);

	// Find errors
	Matrix output_errors = output.subtract(final_outputs);
	Matrix hidden_errors = m_output_weights.transpose().dot(output_errors);

	// Backpropogate
	// m_output_weights = add(
	//		 m_output_weights,
	//     scale(
	// 			  lr,
	//			  dot(
	// 		 			multiply(
	// 						m_output_errors,
	//				  	sigmoidPrime(final_m_outputs)
	//					),
	//					transpose(m_hidden_m_outputs)
	// 				)
	//		 )
	// )

	Matrix sigmoid_primed_mat = final_outputs.sigmoid_prime();
	Matrix multiplied_mat = output_errors.multiply(sigmoid_primed_mat);
	Matrix transposed_mat = hidden_outputs.transpose();
	Matrix dot_mat = multiplied_mat.dot(transposed_mat);
	Matrix scaled_mat = dot_mat.scale(m_learning_rate);
	Matrix added_mat = m_output_weights.add(scaled_mat);


	m_output_weights = added_mat;

	// m_hidden_weights = add(
	// 	 m_hidden_weights,
	// 	 scale (
	//			m_learning_rate
	//    	dot (
	//				multiply(
	//					m_hidden_errors,
	//					sigmoidPrime(m_hidden_m_outputs)
	//				)
	//				transpose(m_inputs)
	//      )
	// 	 )
	// )
	// Reusing variables after freeing memory
	sigmoid_primed_mat = hidden_outputs.sigmoid_prime();
	multiplied_mat = hidden_errors.multiply(sigmoid_primed_mat);
	transposed_mat = input.transpose();
	dot_mat = multiplied_mat.dot(transposed_mat);
	scaled_mat = dot_mat.scale(m_learning_rate);
	added_mat = m_hidden_weights.add(scaled_mat);

	m_hidden_weights = added_mat;
}

void NeuralNetwork::train_batch_imgs(std::vector<Img> imgs, int batch_size)
{
	for (int i = 0; i < batch_size; i++)
	{
		if (i % 100 == 0) 
			printf("Img No. %d\n", i);

		Img cur_img = imgs[i];
		Matrix img_data = Matrix(*cur_img.img_data);
		img_data.flatten(false); // 0 = flatten to column vector
		Matrix output = Matrix(10, 1);
		output.m_entries[cur_img.label][0] = 1; // Setting the result
		train(img_data, output);
	}
}

Matrix NeuralNetwork::predict_img(Img img)
{
	img.img_data->flatten(false);
	Matrix res = predict(*img.img_data);
	return res;
}

double NeuralNetwork::predict_imgs(std::vector<Img> imgs, int n)
{
	int n_correct = 0;
	for (int i = 0; i < n; i++)
	{
		Matrix prediction = predict_img(imgs[i]);
		if (prediction.arg_max() == imgs[i].label)
		{
			n_correct++;
		}
	}
	return 1.0 * n_correct / n;
}

Matrix NeuralNetwork::predict(Matrix input_data)
{
	Matrix hidden_inputs = m_hidden_weights.dot(input_data);
	Matrix hidden_outputs = hidden_inputs.apply(sigmoid);
	Matrix final_inputs = m_output_weights.dot(hidden_outputs);
	Matrix final_outputs = final_inputs.apply(sigmoid);
	Matrix result = final_outputs.soft_max();
	return result;
}

void NeuralNetwork::save(std::string file_string)
{
	mkdir(file_string.c_str(), 0777);
	// Write the descriptor file
	chdir(file_string.c_str());
	FILE *descriptor = fopen("descriptor", "w");
	fprintf(descriptor, "%d\n", m_input);
	fprintf(descriptor, "%d\n", m_hidden);
	fprintf(descriptor, "%d\n", m_output);
	fclose(descriptor);
	m_hidden_weights.save("m_hidden");
	m_output_weights.save("m_output");
	// printf("Successfully written to '%s'\n", file_string);
	std::cout << "Successfully written to: " << file_string << std::endl;
	chdir("-"); // Go back to the orignal directory
}

void NeuralNetwork::print()
{
	printf("# of m_inputs: %d\n", m_input);
	printf("# of m_hidden: %d\n", m_hidden);
	printf("# of m_output: %d\n", m_output);
	printf("m_hidden Weights: \n");
	m_hidden_weights.print();
	printf("m_output Weights: \n");
	m_output_weights.print();
}