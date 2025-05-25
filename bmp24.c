#include "bmp24.h"

/*
 * This file is the file containing all the functions made to get color image filters
 * It provides :
 * Loading
 * Saving
 * Applying filter
 * !!!!! THIS FILE HAS AN ERROR
 * I and my mate tried to fix it, but it seems an error occurs saving an image.
 * It is not due to filter functions, since loading and saving a new image already does the error
 * When reading the properties of the new image, we can see that no width, height, color depth nor dimension is provided.
 * This must be the location of the error.
 * We tried printing image information (width, height , etc.) in multiple lines of the code, with no results.
 * It seems these informations are right in the code, but wrong in the BMP file.
 * Otherwise, the code seems good working and functional. */

t_pixel ** bmp24_allocateDataPixels (int width, int height){ // Allocates memory for pixels
    t_pixel ** pixels = (t_pixel**) malloc( height * sizeof(t_pixel*)); // Allocates memory for the matrix of pixels
    if (pixels == NULL)
    {
      printf("Failed to allocate the DataPixels");
      return NULL;
    }
    for ( int i = 0; i < height ; i++)
    {
      pixels[i] = (t_pixel*) malloc( width * sizeof(t_pixel));
      if (pixels[i] == NULL)
      {
         printf("Failed to allocate the DataPixels");
         return NULL;
      }
    }
    return pixels;
}

void bmp24_freeDataPixels(t_pixel ** pixels, int height) { // Frees memory allocated for a 2D pixel array

	for(int i = 0; i < height; i++) {
		free(pixels[i]); // Free each row of pixel data
	}

	free(pixels); // Free the array of row pointers
}


t_bmp24 * bmp24_allocate(int width, int height, int colorDepth) { // Allocates and initializes a BMP 24-bit image structure

	t_bmp24 * img = (t_bmp24*) malloc(sizeof(t_bmp24)); // Allocate memory for the image structure
	if (img == NULL) {
		printf("Failed to allocate the memory of the 24image");
		return NULL;
	}

	img->data = bmp24_allocateDataPixels(width, height); // Allocate memory for the pixel data
	if (img->data == NULL) {
		printf("Failed to allocate the data memory of the 24image");
		bmp24_free(img); // Free previously allocated structure if pixel data allocation fails
		return NULL;
	}

	// Set image properties
	img->width = width;
	img->height = height;
	img->colorDepth = colorDepth;

	return img; // Return pointer to the allocated image
}


void bmp24_free(t_bmp24 * img) { // Frees all memory associated with a BMP 24-bit image

	if (img == NULL) {
		return; // Nothing to free if the image is NULL
	}

	bmp24_freeDataPixels(img->data, img->height); // Free the 2D pixel data
	free(img); // Free the image structure itself
}



void file_rawRead(uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) { // Reads raw binary data from a file starting at a specific position

	fseek(file, position, SEEK_SET); // Move the file pointer to the specified position
	fread(buffer, size, n, file);    // Read 'n' elements of 'size' bytes into the buffer
}


void file_rawWrite(uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) { // Writes raw binary data to a file at a specific position

	fseek(file, position, SEEK_SET); // Move the file pointer to the specified position
	fwrite(buffer, size, n, file);   // Write 'n' elements of 'size' bytes from the buffer to the file
}


void bmp24_readPixelValue(t_bmp24 * image, int x, int y, FILE * file) { // Reads a single pixel from a file and stores it in the image structure

	uint8_t bgr[3];
	fread(bgr, sizeof(uint8_t), 3, file);

	image->data[y][x].blue  = bgr[0]; // Assign blue value
	image->data[y][x].green = bgr[1]; // Assign green value
	image->data[y][x].red   = bgr[2]; // Assign red value
}

void bmp24_readPixelData(t_bmp24 * image, FILE * file) { // Reads all pixel data from the file into the image structure

	int height = image->height;
	int width = image->width;

	for (int y = height - 1; y >= 0; y--) { // BMP files store pixel rows bottom to top
		for (int x = 0; x < width; x++) {
			bmp24_readPixelValue(image, x, y, file);
		}
	}
}

void bmp24_writePixelValue(t_bmp24 * image, int x, int y, FILE * file) { // Writes a single pixel (BGR format) from the image to the file

	uint8_t bgr[3];

	bgr[0] = image->data[y][x].blue;   // Set blue component
	bgr[1] = image->data[y][x].green;  // Set green component
	bgr[2] = image->data[y][x].red;    // Set red component

	fwrite(bgr, sizeof(uint8_t), 3, file);
}

void bmp24_writePixelData(t_bmp24 * image, FILE * file) { // Writes all pixel data from the image structure to the file

	int height = image->height;
	int width = image->width;

	for (int y = height - 1; y >= 0; y--) { // Write from bottom row to top
		for (int x = 0; x < width; x++) {
			bmp24_writePixelValue(image, x, y, file);
		}
	}
}


t_bmp24 * bmp24_loadImage(const char * filename) {
    // Open the file in binary read mode
    FILE * file = fopen(filename, "rb");
    if (file == NULL) {
        // If the file couldn't be opened, print an error and return NULL
        printf("Error: File %s could not be opened\n", filename);
        return NULL;
    }

    int width;
    int height;
    int colorDepth;


    file_rawRead(BITMAP_WIDTH, &width, sizeof(width), 1, file); // Read the width of the bitmap image from the file

    file_rawRead(BITMAP_HEIGHT, &height, sizeof(height), 1, file); // Read the height of the bitmap image from the file

    file_rawRead(BITMAP_DEPTH, &colorDepth, sizeof(colorDepth), 1, file); // Read the color depth (bits per pixel) from the file

    // Check if width and height are multiples of 4, required for this implementation
    if (width % 4 != 0 || height % 4 != 0) {
        printf("Error: Width and height must be multiples of 4 image not supported \n");
        fclose(file);  // Close the file before returning
        return NULL;
    }

    // Allocate memory for the image struct based on width, height and color depth
    t_bmp24 * img = bmp24_allocate(width, height, colorDepth);

    // Read the bitmap header (file signature and basic info)
    t_bmp_header header;
    file_rawRead(BITMAP_MAGIC, &header, sizeof(t_bmp_header), 1, file);

    // Read the bitmap info header (detailed info like size, planes, compression)
    t_bmp_info header_info;
    file_rawRead(HEADER_SIZE, &header_info, sizeof(t_bmp_info), 1, file);

    // Store the headers in the allocated image struct
    img->header = header;
    img->header_info = header_info;

    // Read the offset in the file where pixel data starts
    uint32_t offset;
    file_rawRead(BITMAP_OFFSET, &offset, sizeof(uint32_t), 1, file);

    // Move file pointer to the start of pixel data based on offset read above
    fseek(file, offset, SEEK_SET);

    bmp24_readPixelData(img, file);

    fclose(file);

    printf("Image loaded successfully !\n");
    return img;
}


void bmp24_saveImage(t_bmp24 * img, const char * filename) {
	// Open file for binary writing
	FILE * file = fopen(filename, "wb");
	if (file == NULL) {
		printf("Error: File %s could not be opened\n", filename);
		return;
	}

	// Write bitmap header and info to file
	file_rawWrite(BITMAP_MAGIC, &img->header, sizeof(t_bmp_header), 1, file);
	file_rawWrite(HEADER_SIZE, &img->header_info, sizeof(t_bmp_info), 1, file);

	// Write pixel data
	bmp24_writePixelData(img, file);

	fclose(file);

	printf("Image saved successfully !\n");
}


void bmp24_negative(t_bmp24 * img) {
	if (img == NULL || img->data == NULL) {
		printf("Error");
		return;
	}

	// Invert each pixel color to create a negative effect
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			img->data[i][j].blue = 255 - img->data[i][j].blue; // Sets black to white
			img->data[i][j].green = 255 - img->data[i][j].green;
			img->data[i][j].red = 255 - img->data[i][j].red;
		}
	}

	printf("Filter applied successfully !\n");
}



void bmp24_grayscale(t_bmp24 * img) {
	if (img == NULL || img->data == NULL) {
		printf("Error");
		return;
	}

	// Convert each pixel to grayscale by averaging RGB values
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			int average = (img->data[i][j].blue + img->data[i][j].green + img->data[i][j].red) / 3;
			// Takes same values
			img->data[i][j].blue = average;
			img->data[i][j].green = average;
			img->data[i][j].red = average;
		}
	}

	printf("Filter applied successfully !\n");
}


uint8_t clamp(int value){ // Clamps the values to [0,255]
    if (value > 255) return 255;
    if (value < 0)   return 0;
    return (uint8_t)value;
}

void bmp24_brightness(t_bmp24 * img, int value) {
	if (img == NULL || img->data == NULL) {
		printf("Error");
		return;
	}

	// Adjust brightness by adding value and clamping each color channel
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			img->data[i][j].blue = clamp(value + img->data[i][j].blue);
			img->data[i][j].green = clamp(value + img->data[i][j].green);
			img->data[i][j].red = clamp(value + img->data[i][j].red);
		}
	}

	printf("Filter applied successfully !\n");
}



t_pixel bmp24_convolution(t_bmp24 *img, int x, int y, float **kernel, int kernelSize) {
	int kOffset = kernelSize / 2;
	float r = 0, g = 0, b = 0;

	// Apply convolution kernel around pixel (x, y)
	for (int i = -kOffset; i <= kOffset; i++) {
		for (int j = -kOffset; j <= kOffset; j++) {
			int xx = x + j;
			int yy = y + i;

			// Check boundaries
			if (xx >= 0 && xx < img->width && yy >= 0 && yy < img->height) {
				t_pixel pixel = img->data[yy][xx];
				float kernelValue = kernel[i + kOffset][j + kOffset];
				r += pixel.red * kernelValue;
				g += pixel.green * kernelValue;
				b += pixel.blue * kernelValue;
			}
		}
	}

	// Clamp color values to valid range
	if (r < 0) r = 0; else if (r > 255) r = 255;
	if (g < 0) g = 0; else if (g > 255) g = 255;
	if (b < 0) b = 0; else if (b > 255) b = 255;

	// Return new pixel after applying convolution
	t_pixel newPixel = {(uint8_t)r, (uint8_t)g, (uint8_t)b};
	return newPixel;
}

// Set the kernels for the filters
float boxBlurKernel[3][3] = {
	{1.0/9.0, 1.0/9.0, 1.0/9.0},
	{1.0/9.0, 1.0/9.0, 1.0/9.0},
	{1.0/9.0, 1.0/9.0, 1.0/9.0}
};

float gaussianBlurKernel[3][3] = {
	{1.0/16.0, 2.0/16.0, 1.0/16.0},
	{2.0/16.0, 4.0/16.0, 2.0/16.0},
	{1.0/16.0, 2.0/16.0, 1.0/16.0}
};

float outlineKernel[3][3] = {
	{-1, -1, -1},
	{-1,  8, -1},
	{-1, -1, -1}
};

float embossKernel[3][3] = {
	{-2, -1, 0},
	{-1,  1, 1},
	{ 0,  1, 2}
};

float sharpenKernel[3][3] = {
	{ 0, -1,  0},
	{-1,  5, -1},
	{ 0, -1,  0}
};

void apply_filter(t_bmp24 *img, float **kernel, int kernelSize) {
	// Allocate temporary buffer for filtered pixels
	t_pixel **tempData = (t_pixel **)malloc(img->height * sizeof(t_pixel *));
	for (int y = 0; y < img->height; y++) {
		tempData[y] = (t_pixel *)malloc(img->width * sizeof(t_pixel));
	}

	// Apply convolution to each pixel, skipping edges
	for (int y = 1; y < img->height - 1; y++) {
		for (int x = 1; x < img->width - 1; x++) {
			tempData[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
		}
	}

	// Copy filtered pixels back and free temporary buffer rows
	for (int y = 1; y < img->height - 1; y++) {
		for (int x = 1; x < img->width - 1; x++) {
			img->data[y][x] = tempData[y][x];
		}
		free(tempData[y]);
	}
	free(tempData);

	printf("Filter applied successfully !\n");
}

// Filters applications
void bmp24_boxBlur(t_bmp24 *img) {
	float *boxBlurKernelFlat[3] = {boxBlurKernel[0], boxBlurKernel[1], boxBlurKernel[2]};
	apply_filter(img, boxBlurKernelFlat, 3);
}

void bmp24_gaussianBlur(t_bmp24 *img) {
	float *gaussianBlurKernelFlat[3] = {gaussianBlurKernel[0], gaussianBlurKernel[1], gaussianBlurKernel[2]};
	apply_filter(img, gaussianBlurKernelFlat, 3);
}

void bmp24_outline(t_bmp24 *img) {
	float *outlineKernelFlat[3] = {outlineKernel[0], outlineKernel[1], outlineKernel[2]};
	apply_filter(img, outlineKernelFlat, 3);
}

void bmp24_emboss(t_bmp24 *img) {
	float *embossKernelFlat[3] = {embossKernel[0], embossKernel[1], embossKernel[2]};
	apply_filter(img, embossKernelFlat, 3);
}

void bmp24_sharpen(t_bmp24 *img) {
	float *sharpenKernelFlat[3] = {sharpenKernel[0], sharpenKernel[1], sharpenKernel[2]};
	apply_filter(img, sharpenKernelFlat, 3);
}

