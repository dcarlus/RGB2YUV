#ifndef __YUVCONVERTER_COLOR__
#define __YUVCONVERTER_COLOR__

#include <intrin.h>


#define THREE_CHANNELS_COLOR 3
#define FOUR_CHANNELS_COLOR 4

typedef struct _Color3 {
	float channels[THREE_CHANNELS_COLOR];
} Color3;

typedef struct _Color4 {
	float channels[FOUR_CHANNELS_COLOR];
} Color4;


typedef struct _Color4SSE {
	__m128 channels;
} Color4SSE;


#endif