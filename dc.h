#ifndef DC_H
#define DC_H
#define CAPTURE_AREA 12
#define BMP_WIDTH 950
#define BMP_HEIGTH 950
#define BMP_CHANNELS 3

void greyscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);
void erodeImage(unsigned char gs_image[BMP_WIDTH][BMP_HEIGTH]); //gs = greyscaled.
char isCell(unsigned char cap_area[CAPTURE_AREA][CAPTURE_AREA]); // Is coordinate an image
void outputImage();// Used at the end of detect cells, to output image with crosses
void detectCells(unsigned char gs_image[BMP_WIDTH][BMP_HEIGTH]);


#endif