#include <iostream>
#include <fstream>
#include <functional>
#include <RGB2YUVUtilities.hpp>


#define MEASURE_CONVERSION_SPEED
#define SAVE_OUTPUTS


/// <summary>
/// Callback to adjust the Y channel value.
/// </summary>
/// <param name="value">
/// The normalized Y value.
/// </param>
/// <returns>
/// Y value on [0;255]
/// </returns>
int AdjustY(float value) {
	return static_cast<int>(value * 255);
}

/// <summary>
/// Callback to adjust the U and V channels value.
/// </summary>
/// <param name="value">
/// The normalized U or V value.
/// </param>
/// <returns>
/// U or V value on [0;255]
/// </returns>
int AdjustUV(float value) {
	return static_cast<int>((value + 1.f) * (255.f / 2.f));
}

/// <summary>
/// Save the YUV image in three PGM files.
/// </summary>
/// <param name="image">
/// The picture to save.
/// </param>
/// <param name="width">
/// Width of the picture.
/// </param>
/// <param name="height">
/// Height of the picture.
/// </param>
void SaveYUV
(
	YUVImage image,
	const int width,
	const int height
) {
	const std::string FileExtension = ".pgm";
	const std::string ChannelFileName[] = { "y", "u", "v" };

	// Simple access to the channels in the following loops.
	float* channels[] = { image.y, image.u, image.v };
	std::function<int(float)> adjustValuesFnc[] = { AdjustY, AdjustUV, AdjustUV };

	// Save the channels in different files.
	for (unsigned char channelIndex = 0; channelIndex < THREE_CHANNELS_COLOR; ++channelIndex) {
		std::string filename = ChannelFileName[channelIndex] + FileExtension;
		std::ofstream outputFile(filename);
		
		if (!outputFile.is_open())
		{
			std::cerr << "Unable to save file " << filename << std::endl;
			return;
		}

		outputFile << "P2" << std::endl;
		outputFile << width << " " << height << std::endl;

		for (unsigned int pixelIndex = 0; pixelIndex < image.size; ++pixelIndex)
		{
			int adjustedChannel = adjustValuesFnc[channelIndex](channels[channelIndex][pixelIndex]);
			outputFile << adjustedChannel << std::endl;
		}

		outputFile.close();
	}
}

int main()
{
	const int AmountThreads = 2;

	RGB2YUVUtilities rgbConverter(AmountThreads);
	rgbConverter.loadPPM("picture.ppm");
	rgbConverter.initializeThreads();

#ifdef MEASURE_CONVERSION_SPEED
	auto start = std::chrono::system_clock::now();
#endif

	YUVImage yuvPicture = rgbConverter.convert();

#ifdef MEASURE_CONVERSION_SPEED
	auto end = std::chrono::system_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Elapsed time = " << elapsedTime.count() << "ms" << std::endl;
#endif

#ifdef SAVE_OUTPUTS
	SaveYUV(yuvPicture, rgbConverter.width(), rgbConverter.height());
#endif

	system("PAUSE");

    return 0;
}

