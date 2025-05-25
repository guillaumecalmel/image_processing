#include "bmp8.cpp"
#include "bmp24.cpp"
#include <stdio.h>
#include <locale>

int display_menu() {
    printf("Please choose an option:\n");
    printf("\t1. Open an image\n");
    printf("\t2. Save an image\n");
    printf("\t3. Apply a filter\n");
    printf("\t4. Display image\n");
    printf("\t5. Quit\n");
    int choice;
    scanf("%d", &choice);
    printf(">>> Your choice: %d\n", choice);
    return choice;
}

int display_filter_choice() {
    printf("Please choose a filter:\n");
    printf("\t1. Negative\n");
    printf("\t2. Brightness\n");
    printf("\t3. Black and white\n");
    printf("\t4. Box blur\n");
    printf("\t5. Gaussian blur\n");
    printf("\t6. Sharpness\n");
    printf("\t7. Outline\n");
    printf("\t8. Emboss\n");
    printf("\t9. Return to the previous menu\n");
    int choice;
    scanf("%d", &choice);
    printf(">>> Your choice: %d\n", choice);
    return choice;
}
int display_filter_choice2() {
    printf("Please choose a filter:\n");
    printf("\t1. Negative\n");
    printf("\t2. Grayscale\n");
    printf("\t3. Brightness\n");
    printf("\t4. Box Blur\n");
    printf("\t5. Gaussian blur\n");
    printf("\t6. Sharpness\n");
    printf("\t7. Outline\n");
    printf("\t8. Emboss\n");
    printf("\t9. Return to the previous menu\n");
    int choice;
    scanf("%d", &choice);
    printf(">>> Your choice: %d\n", choice);
    return choice;
}

void bmp8_img() {
    int value;
    t_bmp8*img;
    int choice = 0;
    while (choice !=5) {
        choice = display_menu();
        if (choice == 1) {
            img = bmp8_loadImage("barbara_gray.bmp");

        }
        if (choice == 2) {
            bmp8_saveImage("new4_barbara_gray.bmp", img);
        }
        if (choice == 3) {
            int choice = display_filter_choice();
            if (choice == 1) {
                bmp8_negative(img);
            }
            if (choice == 2) {
                printf("Choose a brightness value from 0 to 255\n");
                scanf("%d", &value);
                bmp8_brightness(img, value);
            }
            if (choice == 3) {
                printf("Choose a middle value from 0 to 255\n");
                scanf("%d", &value);
                bmp8_threshold(img, value);
            }
            if (choice == 4) {
                bmp8_applyFilter(img, box_blur, 3);
            }
            if (choice == 5) {
                bmp8_applyFilter(img, gaussian_blur, 3);
            }
            if (choice == 6) {
                bmp8_applyFilter(img, sharpen, 3);
            }
            if (choice == 7) {
                bmp8_applyFilter(img, outline, 3);
            }
            if (choice == 8) {
                bmp8_applyFilter(img, emboss, 3);
            }

        }
        if (choice == 4) {
            printf("Image info :\n");
            printf("\tWidth: %d\n", img->width);
            printf("\tHeight: %d\n", img->height);
            printf("\tColor Depth : %d\n", img->colorDepth);
            printf("\tData size : %d\n", img->dataSize);
        }
    }
}

void bmp24_img() {
    int value;
    int choice = 0;
    t_bmp24*img;
    printf("%d\n", img->header.offset);
    while (choice !=5) {
        choice = display_menu();
        if (choice == 1) {
            img = bmp24_loadImage("flowers_color.bmp");
        }
        if (choice == 2) {
            bmp24_saveImage(img, "new_flowers_color.bmp");
        }
        if (choice == 3) {
            int choice = display_filter_choice2();
            if (choice == 1) {
                bmp24_negative(img);
            }
            if (choice == 2) {
                bmp24_grayscale(img);
            }
            if (choice == 3) {
                bmp24_brightness(img, value);
            }
            if (choice == 4) {
                bmp24_boxBlur(img);
            }
            if (choice == 5) {
                bmp24_gaussianBlur(img);
            }
            if (choice == 6) {
                bmp24_sharpen(img);
            }
            if (choice == 7) {
                bmp24_outline(img);
            }
            if (choice == 8) {
                bmp24_emboss(img);
            }
        }
        if (choice == 4) {
            printf("Image info :\n");
            printf("\tWidth: %d\n", img->width);
            printf("\tHeight: %d\n", img->height);
            printf("\tColor Depth : %d\n", img->colorDepth);
            printf("\tData size : %d\n", img->width * img->height);
        }
    }
}

int main(){
    printf("%d\n", box_blur[2][0]);
    int run = 1;
    printf("Welcome to the image processing application !\n");
    while (run != 3) {

        printf("What image do you want to work on ?\n");
        printf("\t1. barbara_gray.bmp (Black and white image)\n");
        printf("\t2. flowers_color.bmp (Colored image)\n");
        printf("\t3. Quit\n");
        scanf("%d", &run);
        if (run == 1) {
            bmp8_img();
        }
        if (run == 2) {
            bmp24_img();
        }
        if (run == 3) {
            printf("Goodbye !\n");
        }
    }



    return 0;

}