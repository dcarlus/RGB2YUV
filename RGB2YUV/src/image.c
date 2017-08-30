#include <image.h>

RGBImage create_rgb(size_t size) {
	RGBImage created;
	created.pixels = calloc(size, sizeof(Color3));
	created.size = size;
	return created;
}

void destroy_rgb(RGBImage image) {
	free(image.pixels);
}


RGBAImage create_rgba(size_t size) {
	RGBAImage created;
	created.pixels = calloc(size, sizeof(Color4));
	created.size = size;
	return created;
}

void destroy_rgba(RGBAImage image) {
	free(image.pixels);
}

RGBAImageSSE convert_rgb_SSE(RGBImage origin) {
	RGBAImageSSE created;
	created.pixels = calloc(origin.size, sizeof(Color4SSE));

	for (size_t color_index = 0; color_index < origin.size; ++color_index) {
		created.pixels[color_index].channels = _mm_setr_ps(
			origin.pixels[color_index].channels[0],
			origin.pixels[color_index].channels[1],
			origin.pixels[color_index].channels[2],
			0.f
		);
	}

	created.size = origin.size;
	return created;
}

RGBAImageSSE create_rgba_SSE(size_t size) {
	RGBAImageSSE created;
	created.pixels = calloc(size, sizeof(Color4SSE));
	created.size = size;
	return created;
}

void destroy_rgba_SSE(RGBAImageSSE image) {
	free(image.pixels);
}


RGBAImageSSE convert_rgba_SSE(RGBAImage origin) {
	RGBAImageSSE created;
	created.pixels = calloc(origin.size, sizeof(Color4SSE));

	for (size_t color_index = 0; color_index < origin.size; ++color_index) {
		created.pixels[color_index].channels = _mm_setr_ps(
			origin.pixels[color_index].channels[0],
			origin.pixels[color_index].channels[1],
			origin.pixels[color_index].channels[2],
			origin.pixels[color_index].channels[3]
		);
	}

	created.size = origin.size;
	return created;
}

YUVImage create_yuv(size_t size) {
	YUVImage created;
	created.y = calloc(size, sizeof(float));
	created.u = calloc(size, sizeof(float));
	created.v = calloc(size, sizeof(float));
	created.size = size;
	return created;
}

void destroy_yuv(YUVImage image) {
	free(image.y);
	free(image.u);
	free(image.v);
}