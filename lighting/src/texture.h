#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

typedef struct image {
	const char *path;
	unsigned format;
} Image;

unsigned *createTextures(Image image[], unsigned count);

#endif
