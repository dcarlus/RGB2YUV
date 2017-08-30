#ifndef __YUVCONVERTER_CONVERSION_MATRIX__
#define __YUVCONVERTER_CONVERSION_MATRIX__

#include <intrin.h>

typedef struct _ConversionMatrix {
	__m128 coefficients[3];
} ConversionMatrix;

/// <summary>
/// Get the matrix for converting yuv to rgb.
/// </summary>
__declspec(dllexport)
ConversionMatrix get_yuv2rgb_matrix();

/// <summary>
/// Get the matrix for converting rgb to yuv.
/// </summary>
__declspec(dllexport)
ConversionMatrix get_rgb2yuv_matrix();

#endif