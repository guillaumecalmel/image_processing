#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * This file is containing all the bmp8 image needs to load and work.
 * It provides :
 * Loading
 * Saving
 * Printing image information
 * Applying filter
 */

t_bmp8 * bmp8_loadImage(const char * filename){ //Loads the image in a structure
  FILE*file = fopen(filename, "rb");
  if (file == NULL){
    printf("Error while openning the file");
    return NULL;
  }

  t_bmp8*image = (t_bmp8*)malloc(sizeof(t_bmp8)); //allocates memory for the image
  if (image == NULL){
    printf("Memory allocation failed");
    return NULL;
  }



  if (fread(image->header, sizeof(unsigned char), 54, file) != 54) { //Check for size error while reading the header
    printf("Error reading BMP header.\n");
    free(image);
    fclose(file);
    return NULL;
  }


  if (image->header[0] != 'B' || image->header[1] != 'M') { //Check if the image is BMP format
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


  if (image->colorDepth != 8) { //Check if the pixels have 8 bits
    printf("This image is not 8-bit grayscale.\n");
    free(image);
    fclose(file);
    return NULL;
  }


  if (fread(image->colorTable, sizeof(unsigned char), 1024, file) != 1024) { //Check for the color Table
    printf("Error reading color table.\n");
    free(image);
    fclose(file);
    return NULL;
  }

  if (image->dataSize == 0) { // Calculate image size if not provided
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
  if (fread(image->data, sizeof(unsigned char), image->dataSize, file) != image->dataSize) { // Check the data size
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

void bmp8_saveImage(const char * filename, t_bmp8 * img){ //This function saves the provided image already loaded in a new bmp file

  FILE*file = fopen(filename, "wb"); //opens file in writing mode
  if(file == NULL){
    printf("Error openning file");
    return;
  }

  // Write header (54 bytes)
  if (fwrite(img->header, sizeof(unsigned char), 54, file) != 54) { //Check if header is 54 bytes
    printf("Error writing BMP header.\n");
    fclose(file);
    return;
  }

  // Write color table (1024 bytes)
  if (fwrite(img->colorTable, sizeof(unsigned char), 1024, file) != 1024) { //Check if color table is 1024 bytes
    printf("Error writing color table.\n");
    fclose(file);
    return;
  }

  // Write all the pixels
  if (fwrite(img->data, sizeof(unsigned char), img->dataSize, file) != img->dataSize) { //Check if the data size is the same
    printf("Error writing image data.\n");
    fclose(file);
    return;
  }

  fclose(file);
  printf("Image saved successfully !\n");
}

void bmp8_free(t_bmp8 * img){ // Frees the memory allocated for the image
  free(img->data); // Frees the memory allocated for the pixels
  free(img); // Frees the memory of the image itself
}

void bmp8_printInfo(t_bmp8 * img){ // Prints the information of the image
  if (img == NULL) {
    printf("Error : No image is loaded\n");
    return;
  }
  printf("Image Info:\n");
  printf("\tWidth: %d\n", img->width);
  printf("\tHeight: %d\n", img->height);
  printf("\tColor Depth: %d\n", img->colorDepth);
  printf("\tData Size: %d\n", img->dataSize);
}

void bmp8_negative(t_bmp8 * img) { // Applies the negative filter
  for (int i = 0; i < img->dataSize; i++) {
    img->data[i] = 255 - img->data[i]; //inversion by setting black to white, and keep going...
  }
  printf("Filter applied successfully!\n");
}

void bmp8_brightness(t_bmp8 * img, int value){ // Applies the brightness filter
   for (int i = 0; i < img->dataSize; i++) {
     int newValue = img->data[i] + value; // Set the pixel to its brightness value
     if (newValue > 255) // Ensures no error
       newValue = 255;
     else if (newValue < 0) // Same
       newValue = 0;

     img->data[i] = newValue;
   }
  printf("Filter applied successfully!\n");
}

void bmp8_threshold(t_bmp8 * img, int threshold){ // Applies the threshold filter, with the threshold value set
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

void bmp8_applyFilter(t_bmp8 * img, float kernel[3][3], int kernelSize) { // Applies filters with a kernel issue
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

      // Apply the kernel
      for (int i = -n; i <= n; i++) {
        for (int j = -n; j <= n; j++) {
          int pixel = img->data[(y + i) * width + (x - j)];

          sum += pixel * kernel[i + n][j + n];
        }
      }

      // Clamp the result to [0, 255]
      if (sum < 0) {
        sum = 0;
      }

      if (sum > 255) {
        sum = 255;
      }

      newData[y * width + x] = sum;
    }
  }

  for (int i = 0; i < img->dataSize; i++) {
    img->data[i] = newData[i];
  }
  printf("Filter applied successfully!\n");
}

