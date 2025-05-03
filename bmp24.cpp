#include "bmp24.h"
#include <stdio.h>
#include <stdlib.h>

t_pixel ** bmp24_allocateDataPixels (int width, int height){
    t_pixel** pixels = (t_pixel**)malloc(height*sizeof(t_pixel*)); //allocate rows
    if (pixels == NULL){
      printf("Error while allocating memory for pixels");
      return NULL;
    }

    for (int i = 0; i < height; i++){
      pixels[i] = (t_pixel*)malloc(width*sizeof(t_pixel));
      if (pixels[i] == NULL){
        printf("Error while allocating memory for pixels");

        for (int j = 0; j<i; j++){ //free the previous allocated pixels
          free(pixels[j]);
      }
      free(pixels);
      return NULL;
    }

  }
  return pixels;
}

void bmp24_freeDataPixels (t_pixel ** pixels, int height){
  for (int i = 0; i < height; i++){
    free(pixels[i]);
  }
  free(pixels);
}

t_bmp24 * bmp24_allocate (int width, int height, int colorDepth){
  t_bmp24*img = (t_bmp24*)malloc(sizeof(t_bmp24)); //allocate memory for the image
  if (img == NULL){
    printf("Error while allocating memory for img");
    return NULL;
  }

  img->data = bmp24_allocateDataPixels(width, height);
  img->width = width;
  img->height = height;
  img->colorDepth = colorDepth;
  return img;
}

void bmp24_free (t_bmp24 * img){
  bmp24_freeDataPixels(img->data, img->height);
  free(img->data);
  free(img);
}

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
  fseek(file, position, SEEK_SET);
  fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
  fseek(file, position, SEEK_SET);
  fwrite(buffer, size, n, file);
}

void bmp24_readPixelValue (t_bmp24 * img, int x, int y, FILE * file){
  int invertedY = img->height - 1 - y; //BMP stores rows bottom to top
  uint32_t pixelOffset = img->header.offset + (invertedY*img->width + x)*3;
  uint8_t bgr[3];

  file_rawRead(pixelOffset, bgr, 1, 3, file);
  img->data[y][x].blue = bgr[0];
  img->data[y][x].green = bgr[1];
  img->data[y][x].red = bgr[2];
}



void bmp_24_readPixelData (t_bmp24 * img, FILE * file){
  for (int y = 0; y < img->height; y++){
    for (int x = 0; x < img->width; x++){
      bmp24_readPixelValue(img, x, y, file);
    }
  }
}

void bmp24_writePixelValue (t_bmp24 * img,int x, int y, FILE * file){
   int invertedY = img->height - 1 - y;
    uint32_t pixelOffset = img->header.offset + (invertedY * img->width + x) * 3;

    uint8_t bgr[3] = {
        img->data[y][x].blue,
        img->data[y][x].green,
        img->data[y][x].red
    };

    file_rawWrite(pixelOffset, bgr, 1, 3, file);
}

void bmp24_writePixelData(t_bmp24*img, FILE*file){
  for (int y = 0; y < img->height; y++){
    for (int x = 0; x < img->width; x++){
      bmp24_writePixelValue(img, x, y, file);
    }
  }
}

t_bmp24 * bmp24_loadImage (const char * filename){

}













