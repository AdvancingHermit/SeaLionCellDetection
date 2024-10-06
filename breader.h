//
// Created by Oscar Stilling on 01/10/2024.
//

#ifndef BREADER_H
#define BREADER_H

#include "bitOPT.h"
#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
} coordinate;

int min(int a, int b);
int max(int a, int b);

void read_bmp(unsigned char* gs_arr, char path[]);
void write_bmp(unsigned char* gs_arr, char input_path[], char output_path[], coordinate centers[], int16_t cellCount);
void write_gs_bmp(unsigned char* gs_arr, char input_path[], char output_path[], coordinate centers[], int16_t cellCount);
void throw_error(char* message);

#endif
