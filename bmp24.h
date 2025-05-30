#include <stdint.h>
#include <stdio.h>
#ifndef BMP24_H
#define BMP24_H

/*This file is the header file of the bmp24.c file.
* It defines the t_bmp24 structure which later is the image structure,
* and implements the functions used. */

// Offsets for the BMP header
#define BITMAP_MAGIC 0x00 // offset 0
#define BITMAP_SIZE 0x02 // offset 2
#define BITMAP_OFFSET 0x0A // offset 10
#define BITMAP_WIDTH 0x12 // offset 18
#define BITMAP_HEIGHT 0x16 // offset 22
#define BITMAP_DEPTH 0x1C // offset 28
#define BITMAP_SIZE_RAW 0x22 // offset 34

// Magical number for BMP files
#define BMP_TYPE 0x4D42 // 'BM' in hexadecimal

// Header sizes
#define HEADER_SIZE 0x0E // 14 octets
#define INFO_SIZE 0x28 // 40 octets

// Constant for the color depth
#define DEFAULT_DEPTH 0x18 // 24


//Definition of the structures

typedef struct { //information of the header
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

typedef struct { //information of the image
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;

typedef struct { //information of a pixel
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_pixel;

typedef struct { //structure for the image file
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
} t_bmp24;

//Functions
t_pixel ** bmp24_allocateDataPixels (int width, int height);
void bmp24_freeDataPixels (t_pixel ** pixels, int height);
t_bmp24 * bmp24_allocate (int width, int height, int colorDepth);
void bmp24_free (t_bmp24 * img);
t_bmp24 * bmp24_loadImage (const char * filename);


void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file);


void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file);


void bmp24_saveImage (t_bmp24 * img, const char * filename);
void bmp24_readPixelValue (t_bmp24 * img, int x, int y, FILE * file);
void bmp24_readPixelData (t_bmp24 * img, FILE * file);
void bmp24_writePixelValue (t_bmp24 * img, int x, int y, FILE * file);
void bmp24_writePixelData (t_bmp24 * img, FILE * file);
void bmp24_negative (t_bmp24 * img);
#endif //BMP24_H
