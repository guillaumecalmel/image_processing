#include "bmp8.h"
#include <stdio.h>

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

  //Check for size of error while reading the header
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
  image->width = *(unsigned int *)(header + 18);
  image->height = *(unsigned int *)(header + 22);
  image->colorDepth = *(unsigned int*)(header + 28);
  image->dataSize = *(unsigned int*)(header + 34);

  //Check if the pixels have 8 bits
  if (image->colorDepth != 8) {
    printf("Image is not 8-bit grayscale.\n");
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
  printf("Image loaded successfully !");
  return image;
}

void bmp8_saveImage(const char * filename, t_bmp8 * img){

  FILE*file = fopen(filename, "wb");
  if(file == NULL){
    printf("Error openning file");
    return 0;
  }

  // Write header (54 bytes)
  if (fwrite(image->header, sizeof(unsigned char), 54, file) != 54) {
    printf("Error writing BMP header.\n");
    fclose(file);
    return 0;
  }

  // Write color table (1024 bytes)
  if (fwrite(image->colorTable, sizeof(unsigned char), 1024, file) != 1024) {
    printf("Error writing color table.\n");
    fclose(file);
    return 0;
  }

  // Write pixel data
  if (fwrite(image->data, sizeof(unsigned char), image->dataSize, file) != image->dataSize) {
    printf("Error writing image data.\n");
    fclose(file);
    return 0;
  }

  fclose(file);
  printf("Image saved successfully !");
  return 0;
}

void bmp8_free(t_bmp8 * img){
  free(img->data);
  free(img);
}

void bmp8_printInfo(t_bmp8 * img){
  printf("Image Info:\n");
  printf("\tWidth: %d", img->width);
  printf("\tHeight: %d", img->height);
  printf("\tColor Depth: %d", img->colorDepth);
  printf("\tData Size: %d", img->dataSize);
}

void bmp8_negative(t_bmp8 * img){
  for (int i = 0; i < img->dataSize; i++) {
    img->data[i] = 255 - img->data[i];
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
}

void bmp8_threshold(t_bmp8 * img, int threshold){
  for (int i = 0; i < img->dataSize; i++){
    if (img->data[i] < threshold){
      img->data[i] = 0;
    if (img->data[i] > threshold){
      img->data[i] = 255;
    }
}


