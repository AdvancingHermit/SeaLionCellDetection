#include "dc.h"
#include <stdio.h>



unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

void greyscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],
               unsigned char output_image[BMP_WIDTH][BMP_HEIGTH]){
    unsigned char THRESHOLD = 90;
    unsigned char white[] = {255, 255, 255};
    unsigned char black = 0;
    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            if (input_image[x][y][0] > THRESHOLD) {

                output_image[x][y] = 1;

            } else {
                output_image[x][y] = 0;
            }
        }
    }
}
void erodeImage(unsigned char gs_image[BMP_WIDTH][BMP_HEIGTH]){

} //gs = greyscaled.
char isCell(unsigned char cap_area[CAPTURE_AREA][CAPTURE_AREA]){

} // Is coordinate an image
void outputImage(unsigned char gs_image[BMP_WIDTH][BMP_HEIGTH], char *  output_file_path){

    for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            unsigned char color = gs_image[x][y] * 255;
            output_image[x][y][0] = color;
            output_image[x][y][1] = color;
            output_image[x][y][2] = color;
        }
    }
    write_bitmap(output_image, output_file_path);

} // Used at the end of detect cells, to output image with crosses
void detectCells(unsigned char gs_image[BMP_WIDTH][BMP_HEIGTH]){

}