#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "common.h"

unsigned *createTextures(Info *image, unsigned count) {
	if (count < 1 || count > 16)
		return 0;

	stbi_set_flip_vertically_on_load(1);

	unsigned *textures = (unsigned *) malloc(count * sizeof(unsigned));
	assert(textures);
	glGenTextures(count, textures);

	for (int i = 0; i < count; i++) {
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, channels;
		unsigned char *data = stbi_load(image[i].path, &width, &height, &channels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D,
						 0, GL_RGB, width, height,
						 0, image[i].type, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else
			fprintf(stderr, "Error loading texture\n");
		stbi_image_free(data);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return textures;
}

#endif