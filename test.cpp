#include "bmp8.cpp"
#include <stdio.h>

int main(){
  printf("Hello\n");
  t_bmp8* myImage = bmp8_loadImage("barbara_gray.bmp");
  bmp8_printInfo(myImage);
  return 0;
}