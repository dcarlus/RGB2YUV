#include <conversion_matrix.h>

ConversionMatrix get_yuv2rgb_matrix()
{
	ConversionMatrix matrix = 
	{
		{
			_mm_setr_ps( 0.299f,	 0.587f,	 0.114f,		0.f),
			_mm_setr_ps(-0.14713f,	-0.28886f,	 0.436f,		0.f),
			_mm_setr_ps( 0.615f,	-0.51498f,	-0.10001f,		0.f),
		}
	};

	return matrix;
}

ConversionMatrix get_rgb2yuv_matrix()
{
	ConversionMatrix matrix =
	{
		{
			_mm_setr_ps(1.f,	 0.f,		1.13983f,	0.f),
			_mm_setr_ps(1.f,	-0.39465f, -0.58060f,	0.f),
			_mm_setr_ps(1.f,	 2.03211f,	0.f,		0.f)
		}
	};

	return matrix;
}