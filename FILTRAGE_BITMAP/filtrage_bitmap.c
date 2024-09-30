// Filtrage Bitmap
// By Hylst - 1997 
// v0.1
// CHarge une image pour la modifier syuivant différents effets (netteté, flou, contrours, ...)


#include <stdio.h>
#include <stdlib.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include <SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

SDL_Window *window = SDL_CreateWindow("Image Filter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VISIBLE);
SDL_Surface *image_surface = SDL_CreateRGBSurface(SDL_PIXELFORMAT_RGB24, width, height, channels * 8, width * channels);
SDL_DestroySurface(image_surface);

// Matrices pour différents filtres
float blur_kernel[3][3] = {
    {1.0/9, 1.0/9, 1.0/9},
    {1.0/9, 1.0/9, 1.0/9},
    {1.0/9, 1.0/9, 1.0/9}
};

float sharpen_kernel[3][3] = {
    {0, -1, 0},
    {-1, 5, -1},
    {0, -1, 0}
};

// Charger l'image en mémoire
unsigned char *load_image(const char *filename, int *width, int *height, int *channels) {
    unsigned char *image = stbi_load(filename, width, height, channels, 0);
    if (!image) {
        printf("Erreur: Impossible de charger l'image %s\n", filename);
        exit(1);
    }
    return image;
}

// Appliquer la convolution sur l'image
void apply_convolution(unsigned char *image, unsigned char *output, int width, int height, int channels, float kernel[3][3]) {
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            for (int c = 0; c < channels; c++) {
                float pixel_value = 0.0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int pixel_index = ((y + ky) * width + (x + kx)) * channels + c;
                        pixel_value += image[pixel_index] * kernel[ky + 1][kx + 1];
                    }
                }
                int output_index = (y * width + x) * channels + c;
                output[output_index] = (unsigned char)pixel_value;
            }
        }
    }
}

// Sauvegarder l'image
void save_image(const char *filename, unsigned char *image, int width, int height, int channels) {
    stbi_write_png(filename, width, height, channels, image, width * channels);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    int width, height, channels;
    unsigned char *image = load_image(argv[1], &width, &height, &channels);

    // Initialiser SDL pour afficher l'image
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Image Filter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *screen_surface = SDL_GetWindowSurface(window);
    SDL_Surface *image_surface = SDL_CreateRGBSurfaceWithFormatFrom(image, width, height, channels * 8, width * channels, SDL_PIXELFORMAT_RGB24);

    // Afficher l'image originale
    SDL_BlitSurface(image_surface, NULL, screen_surface, NULL);
    SDL_UpdateWindowSurface(window);

    // Application des filtres
    unsigned char *output_image = malloc(width * height * channels);
    apply_convolution(image, output_image, width, height, channels, blur_kernel);

    // Sauvegarder l'image floutée
    save_image("blurred_image.png", output_image, width, height, channels);

    // Libération des ressources
    SDL_FreeSurface(image_surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(image);
    free(output_image);

    return 0;
}
