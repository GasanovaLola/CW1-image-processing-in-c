#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* The RGB values of a pixel. */
struct Pixel {
    uint8_t  red;
    uint8_t  green;
    uint8_t  blue;
};

/* An image loaded from a file. */
struct Image {
    int width;             // The width of the image (number of pixels in a row)
    int height;            // The height of the image (number of pixels in a column)
    struct Pixel *pixels;  // A pointer to an array that holds all the pixel data
};

/* Free an image structure */
void free_image(struct Image *img) {
    if (img == NULL) return;   // If the pointer is NULL, do nothing (prevents crashes)
    free(img->pixels);         // Free the dynamically allocated pixel array
    free(img);                 // Free the Image structure itself
}

/* Load HPDEC Image */
struct Image *load_image(const char *filename) {
    // Open the file for reading in text mode ("r" = read mode)
    FILE *f = fopen(filename, "r");

    // Check if the file opened successfully
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened.\n", filename); // Print error message
        return NULL; // Return NULL to indicate failure
    }

    // Read the image format identifier (should be "HPDEC" for a valid file)
    char format[6]; // Buffer to store up to 5 characters plus a null terminator
    if (fscanf(f, "%5s", format) != 1 || strcmp(format, "HPDEC") != 0) {
        fprintf(stderr, "Unsupported image format in %s.\n", filename); // Print error if format is invalid
        fclose(f);   // Close the file before returning
        return NULL; // Return NULL to indicate failure
    }

    // Read image dimensions: height and width
    int width, height;
    if (fscanf(f, "%d %d", &height, &width) != 2) {
        fprintf(stderr, "Invalid image metadata in %s.\n", filename); // Print error if dimensions are not valid
        fclose(f);   // Close the file
        return NULL; // Return NULL to indicate failure
    }

    // Allocate memory for the Image structure
    struct Image *img = malloc(sizeof(struct Image));
    if (img == NULL) {
        fprintf(stderr, "Memory allocation failed for image.\n"); // Print error message
        fclose(f);    // Close the file
        return NULL;  // Return NULL if memory allocation fails
    }

    // Store image width and height in the allocated structure
    img->width = width;
    img->height = height;

    // Allocate memory for the pixel data (width * height pixels)
    img->pixels = malloc(width * height * sizeof(struct Pixel));
    if (img->pixels == NULL) {
        fprintf(stderr, "Memory allocation failed for pixel data.\n"); // Print error message
        free(img);   // Free previously allocated image structure memory
        fclose(f);   // Close the file
        return NULL; // Return NULL to indicate failure
    }

    // Read pixel values from the file (each pixel has 3 values: red, green, and blue)
    for (int i = 0; i < width * height; i++) {
        // Read the red, green, and blue values of the current pixel from the file.
        // `%hhu` is used to read an 8-bit unsigned integer (uint8_t) for each color channel.
        // If fscanf does not successfully read exactly 3 values (R, G, B), it means:
        //  - The file may be corrupted or incomplete (missing pixel data).
        //  - The file format may be incorrect or not match the expected structure.
        // In case of an error, print an error message, free allocated memory, close the file, and return NULL.
        if (fscanf(f, "%hhu %hhu %hhu", &img->pixels[i].red,
                                        &img->pixels[i].green,
                                        &img->pixels[i].blue) != 3) {
            fprintf(stderr, "Unexpected end of pixel data in %s.\n", filename); // Print error if pixel data is incomplete
            free_image(img); // Free allocated memory (both structure and pixels)
            fclose(f);       // Close the file
            return NULL;     // Return NULL to indicate failure
        }
    }

    fclose(f);  // Close the file
    return img; // Return the pointer to the loaded image
}

/* Save HPDEC Image */
// Function to save an image to a file in HPDEC format.
bool save_image(const struct Image *img, const char *filename) {
    // Open the file for writing ("w" = write mode, overwrites existing files)
    FILE *f = fopen(filename, "w");

    // Check if the file opened successfully
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened for writing.\n", filename); // Print error message
        return false; // Return `false` to indicate failure
    }

    // Write the HPDEC format header and image dimensions (height and width)
    // This ensures that the saved file starts with the correct format
    if (fprintf(f, "HPDEC\n%d %d\n", img->height, img->width) < 0) {
        fprintf(stderr, "Error writing header to %s.\n", filename); // Print error message
        fclose(f);     // Close the file before returning
        return false;  // Return `false` to indicate failure
    }

    // Loop through every pixel in the image and write its RGB values to the file
    for (int i = 0; i < img->width * img->height; i++) {
        // Write red, green, and blue values of each pixel to the file in decimal format
        if (fprintf(f, "%d %d %d\n",
                    img->pixels[i].red, 
                    img->pixels[i].green, 
                    img->pixels[i].blue) < 0) {
            fprintf(stderr, "Error writing pixel data to %s.\n", filename); // Print error message
            fclose(f);     // Close the file before returning
            return false;  // Return `false` to indicate failure
        }
    }

    fclose(f);    // Close the file 
    return true;  // Return `true` to indicate successful saving of the image
}

/* Get a pixel safely */
// Function to safely get a pixel from an image.
// Ensures that x and y coordinates are within the valid range.
// If x or y is out of bounds, it clamps them to the nearest valid position.
struct Pixel get_pixel(const struct Image *img, int x, int y) {
    // If x is less than 0, set it to 0 (prevents accessing negative indexes)
    if (x < 0) x = 0;

    // If x is greater than or equal to the image width, set it to the last column (prevents overflow)
    if (x >= img->width) x = img->width - 1;

    // If y is less than 0, set it to 0 (prevents accessing negative indexes)
    if (y < 0) y = 0;

    // If y is greater than or equal to the image height, set it to the last row (prevents overflow)
    if (y >= img->height) y = img->height - 1;

    // Return the pixel at the (x, y) position in row-major order
    return img->pixels[y * img->width + x];
}

/* Apply 3x3 Blur */
struct Image *apply_BLUR(const struct Image *source) {
    // Check if the input image is NULL. If so, return NULL to avoid crashes.
    if (source == NULL) return NULL;

    // Allocate memory for the new blurred image structure
    struct Image *blurred = malloc(sizeof(struct Image));
    if (blurred == NULL) return NULL; // If allocation fails, return NULL

    // Set the dimensions of the new image to match the original
    blurred->width = source->width;
    blurred->height = source->height;

    // Allocate memory for storing pixel data in the new image
    blurred->pixels = malloc(source->width * source->height * sizeof(struct Pixel));

    if (blurred->pixels == NULL) {
        free(blurred); // Free previously allocated memory if pixel allocation fails
        return NULL;
    }

    // Iterate over every pixel in the image
    for (int y = 0; y < source->height; y++) {
        for (int x = 0; x < source->width; x++) {
            // Variables to store the sum of color values in the 3×3 region
            uint32_t sumR = 0, sumG = 0, sumB = 0;
            int count = 0; // Keeps track of the number of pixels summed

            // Iterate over the 3×3 neighborhood centered at (x, y)
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    // Get the pixel at (x + dx, y + dy), handling out-of-bounds cases
                    struct Pixel p = get_pixel(source, x + dx, y + dy);
                    // Accumulate the color values of the neighboring pixels
                    sumR += p.red;
                    sumG += p.green;
                    sumB += p.blue;
                    count++; // Increment the count for averaging
                }
            }

            // Get a pointer to the current pixel in the new blurred image
            struct Pixel *p = &blurred->pixels[y * source->width + x];

            // Corrected blur: proper averaging of 3x3 pixels
            // Compute the average color by dividing by the number of pixels in the region
            p->red = sumR / count;
            p->green = sumG / count;
            p->blue = sumB / count;

            // Ensure values don't go above 255
            // Ensure that color values don't exceed 255 (maximum value for 8-bit colors)
            if (p->red > 255) p->red = 255;
            if (p->green > 255) p->green = 255;
            if (p->blue > 255) p->blue = 255;

            // Debugging: Print pixel changes for every 10th pixel along x and y axis
            if ((x % 10 == 0) && (y % 10 == 0)) {  
                printf("Pixel[%d, %d]: Before: %d %d %d → After: %d %d %d\n",
                       x, y, source->pixels[y * source->width + x].red,
                       source->pixels[y * source->width + x].green,
                       source->pixels[y * source->width + x].blue,
                       p->red, p->green, p->blue);
            }
        }
    }

    // Return the newly blurred image
    return blurred;
}

/* Compare two images */
// Function to compare two images and count the number of identical and different pixels.
// Takes two `Image` pointers (`img1` and `img2`).
// Prints the number of identical and different pixels.
void apply_COMP(const struct Image *img1, const struct Image *img2) {
    // Check if either image is NULL to avoid dereferencing a null pointer.
    if (img1 == NULL || img2 == NULL) {
        fprintf(stderr, "Error: One or both images are NULL.\n"); // Print error message
        return; // Exit the function as comparison is not possible
    }

    // Check if both images have the same dimensions.
    // If dimensions do not match, the comparison cannot be performed.
    if (img1->width != img2->width || img1->height != img2->height) {
        fprintf(stderr, "Error: Images must have the same dimensions for comparison.\n"); // Print error
        return; // Exit function
    }

    // Initialize counters for identical and different pixels
    int identical = 0, different = 0;

    // Loop through all pixels in the images (assuming row-major order storage)
    for (int i = 0; i < img1->width * img1->height; i++) {
        // Compare the red, green, and blue values of the corresponding pixels in both images
        if (img1->pixels[i].red == img2->pixels[i].red &&
            img1->pixels[i].green == img2->pixels[i].green &&
            img1->pixels[i].blue == img2->pixels[i].blue) {
            identical++; // Increment identical count if all color values match
        } else {
            different++; // Otherwise, increment different count
        }
    }

    // Print the final count of identical and different pixels
    printf("Identical pixels: %d\n", identical);
    printf("Different pixels: %d\n", different);
}

/* Copy an image */
// This function creates a deep copy of an image structure.
// It returns a pointer to a new `struct Image` that is an exact copy of `src`.
// If memory allocation fails, it returns `NULL`.
struct Image *copy_image(const struct Image *src) {
    // Check if the input image is NULL to prevent errors
    if (src == NULL) return NULL;

    // Allocate memory for a new Image structure
    struct Image *copy = malloc(sizeof(struct Image));

    // Check if memory allocation for the new Image structure failed
    if (copy == NULL) return NULL;

    // Copy the width and height from the source image to the new image
    copy->width = src->width;
    copy->height = src->height;

    // Allocate memory for the pixel data in the new image
    copy->pixels = malloc(src->width * src->height * sizeof(struct Pixel));

    // Check if memory allocation for pixels failed
    if (copy->pixels == NULL) {
        free(copy);  // Free the previously allocated Image structure
        return NULL; // Return NULL to indicate failure
    }

    // Copy the pixel data from the source image to the new image
    memcpy(copy->pixels, src->pixels, src->width * src->height * sizeof(struct Pixel));
    return copy; // Return the pointer to the copied image
}

/* Main function */
int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided (should be exactly 3).
    if (argc != 4) {
        fprintf(stderr, "Usage: process REFERENCEFILE INPUTFILE OUTPUTFILE\n");
        return 1; // Exit with error code 1 if incorrect usage.
    }

    // Load the reference image (the original, unmodified image).
    struct Image *ref_img = load_image(argv[1]);
    if (ref_img == NULL) return 1; // Exit if loading fails.

    // Load the input image (the image to be processed).
    struct Image *in_img = load_image(argv[2]);
    if (in_img == NULL) {
        free_image(ref_img); // Free reference image to prevent memory leaks.
        return 1; // Exit if loading fails.
    }

    /* Create a modified copy of the input image */
    struct Image *modified_img = copy_image(in_img);
    if (modified_img == NULL) {
        fprintf(stderr, "Failed to create a modified image.\n");
        free_image(ref_img);
        free_image(in_img);
        return 1; // Exit if copying fails.
    }

    /* Modify a few pixels in the image to simulate a change */
    for (int i = 0; i < 5; i++) {
        // Increase the red component of the first 5 pixels by 50 (with modulo 255 to stay within range)
        modified_img->pixels[i].red = (modified_img->pixels[i].red + 50) % 255;
    }

    /* Save the modified image */
    if (!save_image(modified_img, "HPDEC/bars_modified.hpdec")) {
        fprintf(stderr, "Failed to save modified image.\n");
        free_image(ref_img);
        free_image(in_img);
        free_image(modified_img);
        return 1; // Exit if saving fails.
    }

    /* Compare the reference image with the modified image */
    apply_COMP(ref_img, modified_img);

    /* Apply a blur effect to the input image */
    struct Image *out_img = apply_BLUR(in_img);

    
    if (out_img == NULL) {
        fprintf(stderr, "BLUR process failed.\n");
        free_image(ref_img);
        free_image(in_img);
        free_image(modified_img);
        return 1; // Exit if blurring fails.
    }

    /* Save the blurred image */
    if (!save_image(out_img, argv[3])) {
        fprintf(stderr, "Saving image to %s failed.\n", argv[3]);
    }

    /* Free allocated memory */
    free_image(ref_img);
    free_image(in_img);
    free_image(modified_img);
    free_image(out_img);

    return 0; // Successful execution.
}
