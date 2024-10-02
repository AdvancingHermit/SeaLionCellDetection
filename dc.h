#ifndef DC_H
#define DC_H
#define HALF_AREA 7
#include "breader.h"

void erodeImage(unsigned char* gs_arr, char* done); //gs = greyscaled.
void detectCells(unsigned char* gs_arr, int* cellCount,  coordinate centers[], unsigned char area);
void splitCells(unsigned char* gs_arr);

#endif