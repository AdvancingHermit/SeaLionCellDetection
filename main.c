//To compile (linux/mac): gcc breader.c dc.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc breader.c dc.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "dc.h"
#include <time.h>

#include "breader.h"

//Main function
int main(int argc, char** argv) {
    //argc counts how may arguments are passed
    //argv[0] is a string with the name of the program
    //argv[1] is the first command line argument (input image)
    //argv[2] is the second command line argument (output image)

    //Checking that 2 arguments are passed
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
        exit(1);
    }

    //gs = greyscaled.
    unsigned char* gs_arr = (unsigned char*)calloc(((950*950) / 8) + 1, sizeof(unsigned char));
    if (!gs_arr) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    // Initialize gs_arr from file
    read_bmp(gs_arr, argv[1]);

    coordinate center[305];
    int16_t cellCount = 0;
    char done = 0;

    detectCells(gs_arr, &cellCount, center, HALF_AREA+6);
    splitCells(gs_arr);
    while (!done) {
        erodeImage(gs_arr, &done);
        detectCells(gs_arr, &cellCount, center, HALF_AREA);
    }

    //Save gs_arr to file as bmp
    write_bmp(gs_arr, argv[1], argv[2], center, cellCount);
    printf("Found %d cells", cellCount);
    free(gs_arr);
    return 0;
}