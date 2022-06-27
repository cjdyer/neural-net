#include "img.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 10000

std::vector<Img> csv_to_imgs(std::string file_string, uint16_t number_of_imgs) 
{
	FILE *fp;
	std::vector<Img> imgs;

	char row[MAXCHAR];
	fp = fopen(file_string.c_str(), "r");

	// Read the first line w
	fgets(row, MAXCHAR, fp);
	int i = 0;

	while (feof(fp) != 1 && i < number_of_imgs) 
    {
		int j = 0;
		fgets(row, MAXCHAR, fp);
		char* token = strtok(row, ",");
		Img cur_img;
		cur_img.img_data = new Matrix(28, 28);

		while (token != NULL) 
        {
			if (j == 0) 
            {
				cur_img.label = atoi(token);
			}
            else 
            {
				cur_img.img_data->m_entries[(j-1) / 28][(j-1) % 28] = atoi(token) / 256.0;
			}

			token = strtok(NULL, ",");
			j++;
		}

		imgs.push_back(cur_img);
		i++;
	}

	fclose(fp);
	return imgs;
}

void img_print(Img& img) 
{
	img.img_data->print();
	printf("Img Label: %d\n", img.label);
}