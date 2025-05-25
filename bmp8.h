#ifndef BMP8_H
#define BMP8_H

typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char * data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;

float box_blur[3][3] = {{0.11, 0.11, 0.11}, {0.11, 0.11, 0.11}, {0.11, 0.11, 0.11}};
float gaussian_blur[3][3] = {{0.0625, 0.125,0.0625}, {0.125, 0.25,0.125},{0.0625, 0.125,0.0625}};
float outline[3][3] = {{-1,-1,-1}, {-1,8,-1}, {-1,-1,-1}};
float emboss[3][3] = {{-2, -1, 0}, {-1, 1, 1}, {0, 1, 2}};
float sharpen[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

t_bmp8 * bmp8_loadImage(const char * filename);
void bmp8_saveImage(const char * filename, t_bmp8 * img);
void bmp8_free(t_bmp8 * img);
void bmp8_printInfo(t_bmp8 * img);
void bmp8_negative(t_bmp8 * img);
void bmp8_brightness(t_bmp8 * img, int value);
void bmp8_threshold(t_bmp8 * img, int threshold);
void bmp8_applyFilter(t_bmp8 * img, float kernel[3][3], int kernelSize);



#endif //BMP8_H


