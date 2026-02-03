#include "tgaimage.h"

TGAImage* new_TGAImage(const int w, const int h, const uint8_t bpp) {
	TGAImage* new = malloc(sizeof(*new) + sizeof(uint8_t)*w*h*bpp);
	new->w = w;
	new->h = h;
	new->bpp = bpp;

	return new;
}

bool read_tga_file(const char* fname, TGAImage* image) {
	FILE* in = fopen(fname, "r");
	if (!in) {
		printf("cant open file\n");
		return false;
	}

	TGAHeader header;
	size_t n = fread(&header, 1, sizeof(header), in);
	uint8_t* heada = (uint8_t*)&header;

	for (size_t i = 0; i < n; i++)
		printf("%02X ", heada[i]);

	return true;
}
