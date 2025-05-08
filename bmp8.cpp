#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>

t_bmp8 * bmp8_loadImage(const char * filename){
  FILE*file = fopen(filename, "rb");
  if (file == NULL){
    printf("Error while openning the file");
    return NULL;
  }

  t_bmp8*image = (t_bmp8*)malloc(sizeof(t_bmp8)); //allocates memory for an image
  if (image == NULL){
    printf("Memory allocation failed");
    return NULL;
  }


  //Check for size error while reading the header
  if (fread(image->header, sizeof(unsigned char), 54, file) != 54) {
    printf("Error reading BMP header.\n");
    free(image);
    fclose(file);
    return NULL;
  }

  // Check the BMP signature
  if (image->header[0] != 'B' || image->header[1] != 'M') {
    printf("Not a BMP file.\n");
    free(image);
    fclose(file);
    return NULL;
  }

  //Extracts the fields of the header
  image->width = *(unsigned int *)(image->header + 18);
  image->height = *(unsigned int *)(image->header + 22);
  image->colorDepth = *(unsigned short*)(image->header + 28);
  image->dataSize = *(unsigned int*)(image->header + 34);

  //Check if the pixels have 8 bits
  if (image->colorDepth != 8) {
    printf("This image is not 8-bit grayscale.\n");
    free(image);
    fclose(file);
    return NULL;
  }

  // Read color table (1024 bytes for 8-bit)
  if (fread(image->colorTable, sizeof(unsigned char), 1024, file) != 1024) {
    printf("Error reading color table.\n");
    free(image);
    fclose(file);
    return NULL;
  }

  // Calculate image size if not provided
  if (image->dataSize == 0) {
    image->dataSize = image->width * image->height;
  }

  // Allocate memory for pixel data
  image->data = (unsigned char *)malloc(image->dataSize);
  if (image->data == NULL) {
    printf("Memory allocation for image data failed");
    free(image);
    fclose(file);
    return NULL;
  }

  // Read pixel data
  if (fread(image->data, sizeof(unsigned char), image->dataSize, file) != image->dataSize) {
    printf("Error reading image data.\n");
    free(image->data);
    free(image);
    fclose(file);
    return NULL;
  }

  fclose(file);
  printf("Image loaded successfully !\n");
  return image;
}

void bmp8_saveImage(const char * filename, t_bmp8 * img){

  FILE*file = fopen(filename, "wb");
  if(file == NULL){
    printf("Error openning file");
  }

  // Write header (54 bytes)
  if (fwrite(img->header, sizeof(unsigned char), 54, file) != 54) {
    printf("Error writing BMP header.\n");
    fclose(file);
  }

  // Write color table (1024 bytes)
  if (fwrite(img->colorTable, sizeof(unsigned char), 1024, file) != 1024) {
    printf("Error writing color table.\n");
    fclose(file);
  }

  // Write pixel data
  if (fwrite(img->data, sizeof(unsigned char), img->dataSize, file) != img->dataSize) {
    printf("Error writing image data.\n");
    fclose(file);
  }

  fclose(file);
  printf("Image saved successfully !\n");
}

void bmp8_free(t_bmp8 * img){
  free(img->data);
  free(img);
}

void bmp8_printInfo(t_bmp8 * img){
  printf("Image Info:\n");
  printf("\tWidth: %d\n", img->width);
  printf("\tHeight: %d\n", img->height);
  printf("\tColor Depth: %d\n", img->colorDepth);
  printf("\tData Size: %d\n", img->dataSize);
}

void bmp8_negative(t_bmp8 * img) {
  for (int i = 0; i < img->dataSize; i++) {
    img->data[i] = 255 - img->data[i];
  }
  printf("Filter applied successfully!\n");
}

void bmp8_brightness(t_bmp8 * img, int value){
   for (int i = 0; i < img->dataSize; i++) {
     int newValue = img->data[i] + value;

     if (newValue > 255)
       newValue = 255;
     else if (newValue < 0)
       newValue = 0;

     img->data[i] = newValue;
   }
  printf("Filter applied successfully!\n");
}

void bmp8_threshold(t_bmp8 * img, int threshold){
  for (int i = 0; i < img->dataSize; i++) {
    if (img->data[i] < threshold) {
      img->data[i] = 0;
    }
    if (img->data[i] > threshold){
      img->data[i] = 255;
    }
  }
  printf("Filter applied successfully!\n");
}

void bmp8_applyFilter(t_bmp8 * img, int kernel[3][3], int kernelSize) {
  int height = img->height;
  int width = img->width;
  int n = kernelSize/2; //half the size of the kernel

  unsigned char*newData = (unsigned char*)malloc(img->dataSize); //allocates memory for a copy of the image
  if (newData == NULL) {
    printf("Memory allocation for new data failed");
    return;
  }
  for (int i = 0; i < img->dataSize; i++) { //copy the image for a clean filtering, to not pollute the result
    newData[i] = img->data[i];
  }

  // Traverse image, avoiding edges, by dividing the reading by 2
  for (int y = n; y < height - n; y++) {
    for (int x = n; x < width - n; x++) {
      float sum = 0.0;

      // Convolution: apply the kernel
      for (int i = -n; i <= n; i++) {
        for (int j = -n; j <= n; j++) {
          int pixel = img->data[(y - i) * width + (x - j)];
          //printf("%d", kernel[i+n][j+n]);
          sum += pixel * kernel[i + n][j + n];

        }
      }

      // Clamp the result to [0, 255]
      if (sum < 0) sum = 0;
      if (sum > 255) sum = 255;

      newData[y * width + x] = sum;
    }
  }
  printf("Filter applied successfully!\n");
}

