#ifndef __YUVCONVERTER_IMAGE__
#define __YUVCONVERTER_IMAGE__

#include <color.h>


typedef struct _RGBImage {
	Color3* pixels;
	size_t size;
} RGBImage;

typedef struct _RGBAImage {
	Color4* pixels;
	size_t size;
} RGBAImage;

typedef struct _RGBAImageSSE {
	Color4SSE* pixels;
	size_t size;
} RGBAImageSSE;



typedef struct _YUVImage {
	float* y;
	float* u;
	float* v;
	size_t size;
} YUVImage;


__declspec(dllexport)
RGBImage create_rgb(size_t size);

__declspec(dllexport)
void destroy_rgb(RGBImage image);


__declspec(dllexport)
RGBAImage create_rgba(size_t size);

__declspec(dllexport)
void destroy_rgba(RGBAImage image);


__declspec(dllexport)
RGBAImageSSE create_rgba_SSE(size_t size);

__declspec(dllexport)
void destroy_rgba_SSE(RGBAImageSSE image);

__declspec(dllexport)
RGBAImageSSE convert_rgb_SSE(RGBImage origin);

__declspec(dllexport)
RGBAImageSSE convert_rgba_SSE(RGBAImage origin);


__declspec(dllexport)
YUVImage create_yuv(size_t size);

__declspec(dllexport)
void destroy_yuv(YUVImage image);

#endif