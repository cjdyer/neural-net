#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "img.h"
#include "matrix.h"
#include "nn.h"

int main() {
	srand(time(NULL));

	//TRAINING
	// int number_imgs = 1000;
	// std::vector<Img> imgs = csv_to_imgs("../data/mnist_train.csv", number_imgs);
	// NeuralNetwork net = NeuralNetwork(784, 300, 10, 0.1);
	// net.train_batch_imgs(imgs, number_imgs);
	// net.save("../testing_net");

	// PREDICTING
	int number_imgs = 100;
	std::vector<Img> imgs = csv_to_imgs("../data/mnist_test.csv", number_imgs);
	NeuralNetwork net = NeuralNetwork("../testing_net");
	double score = net.predict_imgs(imgs, 100);
	printf("Score: %1.5f\n", score);

	return 0;
}