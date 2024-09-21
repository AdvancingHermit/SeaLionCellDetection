#ifndef DC_H
#define DC_H
#define HALF_AREA 7
#include "cbmp.h"

struct coordinate {
    int x;
    int y;
};

void greyscale(unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS], unsigned char (*gs_image)[BMP_HEIGTH]);
void erodeImage(unsigned char (*gs_image)[BMP_HEIGTH]); //gs = greyscaled.
void outputImage(unsigned char (*input_image)[BMP_HEIGTH][BMP_CHANNELS],
                    char* output_file_path,struct coordinate centers[],
                    int* cellCount);// Used at the end of detect cells, to output image with crosses
void detectCells(unsigned char (*gs_image)[BMP_HEIGTH], int* cellCount,  struct coordinate centers[]);


#endif