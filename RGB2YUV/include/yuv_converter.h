#ifndef __YUVCONVERTER_YUV__
#define __YUVCONVERTER_YUV__

#include <color.h>
#include <image.h>
#include <conversion_matrix.h>

/// <summary>
/// Convert a RGB image to YUV.
/// </summary>
__declspec(dllexport)
void convert_rgb2yuv(RGBAImageSSE rgba_image, YUVImage* yuv_image);

void _convert_rgb2yuv(const ConversionMatrix* matrix, RGBAImageSSE rgba_image, YUVImage* yuv_image);

#endif
