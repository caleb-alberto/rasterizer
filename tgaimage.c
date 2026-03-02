#include "tgaimage.h"

TGAImage *new_TGAImage(int w, int h, uint8_t bpp) {
    if (w <= 0 || h <= 0 || bpp == 0)
        return NULL;

    size_t nbytes = (size_t)w * h * bpp;

    TGAImage *img = malloc(sizeof(*img));
    if (!img)
        return NULL;

    img->data = malloc(nbytes);
    if (!img->data) {
        free(img);
        return NULL;
    }

    img->w = w;
    img->h = h;
    img->bpp = bpp;

    return img;
}

bool read_tga_file(const char* fname, TGAImage* image) {
	FILE* in = fopen(fname, "r");
	if (!in) {
		printf("cant open file\n");
		return false;
	}

        TGAHeader header;
        size_t n = fread(&header, 1, sizeof(header), in);
        if (n != sizeof(header)) {
                printf("an error occured while reading the header\n");
                fclose(in);
                return false;
        }

        image->w = header.width;
        image->h = header.height;
        image->bpp = header.bitsperpixel >> 3;

        if (image->w <= 0 || image->h <= 0 ||
            (image->bpp != GRAYSCALE &&
             image->bpp != RGB &&
             image->bpp != RGBA)) {
                printf("bad bpp (or width/height) value\n");
                fclose(in);
                return false;
        }

        size_t nbytes = image->w * image->h * image->bpp;
        image->data = malloc(nbytes);
        if (!data) {
                in.close();
                return false;
        }
        
        memset(image->data, 0, nbytes);

        if (3 == header.datatypecode || 2 == header.datatypecode) {
                if (fread(image->data, 1, nbytes, in) != nbytes) {
                        printf("an error occured while reading the data\n");
                        return false;
                }
        } else if (header.datatypecode == 10 || header.datatypecode == 11) {
                if (!load_rle_data(in, image)) {
                        printf("an error occured while reading the data\n");
                        free(image->data);
                        fclose(in);
                        return false;
                }

        } else {
                printf("unsupported file format %d\n", header.datatypecode);
                free(image->data);
                fclose(in);
                return false;
        }

        if (!(header.imagedescriptor & 0x20))
                flip_vertically(image);

        if (header.imagedescriptor & 0x10)
                flip_horizontally(image);

        printf("%dx%d/%d\n", image->w, image->h, image->bpp * 8);

        fclose(in);
        return true;
}

bool load_rle_data(FILE* in, TGAImage* image) {
        size_t pixelcount = image->w * image->h;
        size_t currentpixel = 0;
        size_t currentbyte = 0;
        TGAColor colorbuffer;
    
        do {
                uint8_t chunkheader = 0;
                int c = fgetc(in);
                if (c == EOF) {
                        fprintf(stderr, "an error occured while reading the data\n");
                        return false;
                }
                chunkheader = (uint8_t)c;
        
                if (chunkheader < 128) {
                        chunkheader++;
                        for (int i = 0; i < chunkheader; i++) {
                                size_t n = fread(colorbuffer.bgra, 1, image->bpp, in);
                                if (n != image->bpp) {
                                        fprintf(stderr, "an error occured while reading the header\n");
                                        return false;
                                }
                                for (int t = 0; t < image->bpp; t++)
                                        image->data[currentbyte++] = colorbuffer.bgra[t];
                                currentpixel++;
                                
                                if (currentpixel > pixelcount) {
                                        fprintf(stderr, "Too many pixels read\n");
                                        return false;
                                }
                        }
                } else {
                        chunkheader -= 127;
                        size_t n = fread(colorbuffer.bgra, 1, image->bytespp, in);
                        if (n != image->bytespp) {
                                fprintf(stderr, "an error occured while reading the header\n");
                                return false;
                        }
                        for (int i = 0; i < chunkheader; i++) {
                                for (int t = 0; t < image->bytespp; t++) {
                                        image->data[currentbyte++] = colorbuffer.bgra[t];
                                }
                                currentpixel++;
                                if (currentpixel > pixelcount) {
                                        fprintf(stderr, "Too many pixels read\n");
                                        return false;
                                }
                        }
                }
        } while (currentpixel < pixelcount);
    
        return true;
}
