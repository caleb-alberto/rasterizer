#include "tgaimage.h"

int main() {
	TGAImage* img = new_TGAImage(16,16,16);
	bool yes = read_tga_file("framebuffer.tga", img);
        return 0;
}
