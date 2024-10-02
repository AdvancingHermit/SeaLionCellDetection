#ifndef DC_H
#define DC_H
#define HALF_AREA 7
#include "breader.h"






void setInputImage(unsigned char inputImageGet[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);
unsigned char (*getInputImageDraw())[BMP_HEIGTH][BMP_CHANNELS];
void erodeImage(unsigned char* gs_arr, char* done, coordinate marked[], int *counter); //gs = greyscaled.
void outputImage(unsigned char* gs_arr,
                    char* output_file_path,coordinate centers[],
                    int* cellCount);// Used at the end of detect cells, to output image with crosses
void detectCells(unsigned char* gs_arr, int* cellCount,  coordinate centers[], unsigned char area);
void splitCells(unsigned char* gs_arr);
void outputGSImage(unsigned char* gs_arr,char*  output_flie_path);


#endif