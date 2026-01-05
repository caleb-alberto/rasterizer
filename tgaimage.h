#ifndef TGAIMAGE
#define TGAIMAGE
#include <stdint.h>
#include <stdio.h>

#pragma pack(push,1)
typedef struct TGAHeader {
        uint8_t  idlength = 0;
        uint8_t  colormaptype = 0;
        uint8_t  datatypecode = 0;
        uint16_t colormaporigin = 0;
        uint16_t colormaplength = 0;
        uint8_t  colormapdepth = 0;
        uint16_t x_origin = 0;
        uint16_t y_origin = 0;
        uint16_t width = 0;
        uint16_t height = 0;
        uint8_t  bitsperpixel = 0;
        uint8_t  imagedescriptor = 0;
} TGAHeader;
#pragma pack(pop)

typedef struct TGAColor {
        uint8_t bgra[4] = {0,0,0,0};
        uint8_t bytespp = 4;
} TGAColor;

typedef struct TGAImage {
        enum Format { GRAYSCALE=1, RGB=3, RGBA=4 };
        int w = 0;
        int h = 0;
        uint8_t bpp = 0;
} TGAImage;

TGAImage* new_TGAImage(const int w, const int h, const uint8_t bpp);
bool read_tga_file(const char* fname, TGAImage* image);
bool write_tga_file(const char* fname, const bool vflip=true, const bool rle=true, TGAImage* image);
bool load_rle_data(FILE* in, TGAImage* image);
bool unload_rle_data(FILE* out, TGAImage* image);
TGAColor get(const int x, const int y, const uint8_t bpp);
void set(const int x, const int y, const TGAColor* c, TGAImage* image);
void flip_horizontally(TGAImage* image);
void flip_vertically(TGAImage* image);
int width(TGAImage* image);
int height(TGAImage* image);

#endif
