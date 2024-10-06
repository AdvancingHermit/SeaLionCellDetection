#ifndef DC_H
#define DC_H
#define HALF_AREA 7

#include "breader.h"

void erodeImage(unsigned char* gs_arr, char* done);
void detectCells(unsigned char* gs_arr, int16_t* cellCount,  coordinate centers[], unsigned char area);
void splitCells(unsigned char* gs_arr);

#endif