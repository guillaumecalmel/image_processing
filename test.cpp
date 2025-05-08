#include "bmp8.cpp"
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
            bmp8_saveImage("new_barbara_gray.bmp", img);
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
        if (run == 3) {
            printf("Goodbye !\n");
        }
    }



    return 0;

}