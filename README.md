# Image Processing Application

Hello ! Welcome in the README file of this project !

## Overview

This is a simple command-line image processing application written in C, 
designed to work with BMP images in both 8-bit grayscale and 24-bit color formats. 
The program supports loading, saving, displaying basic image information, 
and applying various filters and effects to images.

---

## Features

- Load and save BMP images:
    - 8-bit grayscale BMP images (`barbara_gray.bmp`)
    - 24-bit color BMP images (`flowers_color.bmp`)
- Apply image filters such as:
    - Negative
    - Brightness adjustment
    - Black and white thresholding (8-bit images)
    - Grayscale conversion (24-bit images)
    - Box blur
    - Gaussian blur
    - Sharpen
    - Outline
    - Emboss
- Display image information including width, height, color depth, and data size
- Menu-driven interface for easy navigation

---

## File Structure

- `bmp8.c` / `bmp8.h`: Functions and structures for handling 8-bit grayscale BMP images
- `bmp24.c` / `bmp24.h`: Functions and structures for handling 24-bit color BMP images
- `main.c`: Main application file with menu interface and logic to use bmp8 and bmp24 functionalities

---

## Filters Details

### For 8-bit grayscale images

- Negative
- Brightness adjustment (0-255)
- Thresholding (convert to black and white)
- Box blur
- Gaussian blur
- Sharpen
- Outline
- Emboss

### For 24-bit color images

- Negative
- Grayscale
- Brightness adjustment (0-255)
- Box blur
- Gaussian blur
- Sharpen
- Outline
- Emboss

---

## Example

```plaintext
Welcome to the image processing application !
What image do you want to work on ?
    1. barbara_gray.bmp (Black and white image)
    2. flowers_color.bmp (Colored image)
    3. Quit
>>> Your choice: 1

Please choose an option:
    1. Open an image
    2. Save an image
    3. Apply a filter
    4. Display image
    5. Quit
>>> Your choice: 1
Loading barbara_gray.bmp...
```

## Dependencies

- Standard C library (`stdio.h`)
- BMP processing implemented in `bmp8.c` and `bmp24.c`

---

## Notes

- The program currently supports only these two BMP images for simplicity.
- Make sure the BMP files are present in the same directory as the executable.

---





