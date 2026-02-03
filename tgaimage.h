#ifndef TGAIMAGE     
#define TGAIMAGE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#pragma pack(push,1)
typedef struct {
	uint8_t  idlength;
	uint8_t  colormaptype;
	uint8_t  datatypecode;
	uint16_t colormaporigin;
	uint16_t colormaplength;
	uint8_t  colormapdepth;
	uint16_t x_origin;
	uint16_t y_origin;
	uint16_t width;
	uint16_t height;
	uint8_t  bitsperpixel;
	uint8_t  imagedescriptor;;
} TGAHeader;
#pragma pack(pop)

typedef struct {
        uint8_t bgra[4];
        uint8_t bytespp;
} TGAColor;

typedef enum { GRAYSCALE=1, RGB=3, RGBA=4 } Format ;

typedef struct {
        int w;
        int h;
        uint8_t bpp;
	uint8_t data[];
} TGAImage;

TGAImage* new_TGAImage(const int w, const int h, const uint8_t bpp);
bool read_tga_file(const char* fname, TGAImage* image);
bool write_tga_file(const char* fname, const bool vflip, const bool rle, TGAImage* image);
bool load_rle_data(FILE* in, TGAImage* image);
bool unload_rle_data(FILE* out, TGAImage* image);
TGAColor get(const int x, const int y, const uint8_t bpp);
void set(const int x, const int y, const TGAColor* c, TGAImage* image);
void flip_horizontally(TGAImage* image);
void flip_vertically(TGAImage* image);
int width(TGAImage* image);
int height(TGAImage* image);

#endif
