//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c dc.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include "dc.h"





//Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char gs_image[BMP_WIDTH][BMP_HEIGTH];

//Main function
int main(/*int argc, char** argv*/) {
    /*
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

    printf("Example program - 02132 - A1\n");*/


    //Load image from file

   // char input_path[] = "samples/easy/3easy.bmp";
   // char input_path[] = "samples/impossible/1impossible.bmp";
//   char input_path[] = "samples/medium/1medium.bmp";
    char input_path[] = "samples/hard/1hard.bmp";

    char output_path[] = "output/output.bmp";
    struct coordinate center[1000];


    read_bitmap(input_path, input_image);
    //greyscale image
    greyscale(input_image, gs_image);

    setInputImage(input_image);

    //erode image
    splitCells(gs_image);
    int cellCount = 0;
    for (int i = 35; i > 0; i--) {
        erodeImage(gs_image);
        detectCells(gs_image, &cellCount, center);
       // printf("%u", cellCount);
    }

    printf("%d", cellCount);
    //Save image to file
    outputImage(getInputImageDraw(), output_path, center, &cellCount);

    printf("Done!\n");
    return 0;
}