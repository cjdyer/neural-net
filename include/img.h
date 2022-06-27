#ifndef IMG_H
#define IMG_H

#include "matrix.h"
#include <vector>

class Img 
{
public:
	Img() {};
	Matrix* img_data;
	int label;
};

std::vector<Img> csv_to_imgs(std::string file_string, uint16_t number_of_imgs);
void img_print(Img& img);

#endif // IMG_H