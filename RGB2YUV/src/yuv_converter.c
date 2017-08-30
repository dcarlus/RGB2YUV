#include <yuv_converter.h>

void convert_rgb2yuv
(
	RGBAImageSSE rgba_image, 
	YUVImage* yuv_image
)
{
	ConversionMatrix matrix = get_rgb2yuv_matrix();
	_convert_rgb2yuv(&matrix, rgba_image, yuv_image);
}

void _convert_rgb2yuv
(
	const ConversionMatrix* matrix,
	RGBAImageSSE rgba_image,
	YUVImage* yuv_image
)
{
	// Make access to the channels much more simpler in the next loop.
	float* yuv_channels[] = {
		yuv_image->y,
		yuv_image->u,
		yuv_image->v
	};

	for (size_t color_index = 0; color_index < rgba_image.size; ++color_index) {
		for (int component_index = 0; component_index < THREE_CHANNELS_COLOR; ++component_index) {
			__m128 image_color = rgba_image.pixels[color_index].channels;
			__m128 matrix_row = matrix->coefficients[component_index];

			// 0111 indicates the values on which the product is made
			// (here we have a Vector3 so we ignore the last value, the
			// one at the highest position in the __m128 structure).

			// We then store the result of the product into the first
			// index of the resulting __m128 structure. We get it by a
			// conversion to an array (no extra instruction with
			// reinterpret_cast).
			__m128 conversion_result = _mm_dp_ps(
				image_color,
				matrix_row,
				0b0111'0001
			);

			float* result_array = (float*)(&conversion_result);
			yuv_channels[component_index][color_index] = (result_array[0]);
		}
	}
}