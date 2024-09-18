#ifndef DC_H
#define DC_H
#define HALF_AREA 7
#include "cbmp.h"

void greyscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]);
void erodeImage(unsigned char (*gs_image)[BMP_HEIGTH]); //gs = greyscaled.
void outputImage( unsigned char gs_image[BMP_WIDTH][BMP_HEIGTH], char * output_file_path);// Used at the end of detect cells, to output image with crosses
void detectCells(unsigned char (*gs_image)[BMP_HEIGTH], int* cellCount);


#endif