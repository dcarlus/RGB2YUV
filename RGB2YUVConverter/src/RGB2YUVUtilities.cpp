#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>

#include <RGB2YUVUtilities.hpp>

using namespace std::chrono_literals;

RGB2YUVUtilities::RGB2YUVUtilities(const unsigned int amountThreads)
{
	_threadPool.resize(amountThreads);
	_completedThreads = amountThreads;
}

RGB2YUVUtilities::~RGB2YUVUtilities()
{
	destroy_rgba_SSE(_sourceRGB);
}


void RGB2YUVUtilities::loadPPM(const std::string& rgbFilePath)
{
	std::ifstream ppmFile(rgbFilePath);

	if (!ppmFile.is_open())
	{
		std::cerr << "Unable to open " << rgbFilePath << std::endl;
		return;
	}

	// Get the header.
	std::string header;
	ppmFile >> header;

	// Get width and height of the picture.
	ppmFile >> _width >> _height;

	_amountPixels = _width * _height;
	RGBImage readPixelsData = create_rgb(_amountPixels);

	for (size_t pixelIndex = 0; pixelIndex < _amountPixels; ++pixelIndex)
	{
		for (int channelIndex = 0; channelIndex < THREE_CHANNELS_COLOR; ++channelIndex)
		{
			int channelValue;
			ppmFile >> channelValue;
			readPixelsData.pixels[pixelIndex].channels[channelIndex] = channelValue / 255.f;
		}
	}

	_sourceRGB = convert_rgb_SSE(readPixelsData);
	destroy_rgb(readPixelsData);

	ppmFile.close();


	// Create the YUV picture from the dimensions of the RGB one.
	_resultYUV = create_yuv(_amountPixels);
}

void RGB2YUVUtilities::initializeThreads()
{
	size_t amountThreads = _threadPool.size();
	//for (size_t pictureIndex = 0 ; pictureIndex < amountThreads; ++pictureIndex)
	//{
	//	destroy_rgba_SSE(_threadPicture[pictureIndex]);
	//}

	for (size_t threadIndex = 0; threadIndex < amountThreads; ++threadIndex)
	{
		auto newThread = std::thread
		(
			&RGB2YUVUtilities::conversion_task,
			this,
			threadIndex
		);

		newThread.detach();
	}
}

YUVImage RGB2YUVUtilities::convert()
{
	const int MaxBenchLoops = 1'000;

	size_t amountThreads = _threadPool.size();
	if (amountThreads > 0) {
		for (int i = 0; i < MaxBenchLoops; ++i)
		{
			// Update the subpart of the picture for each thread.
			size_t sizeParts = _sourceRGB.size / amountThreads;

			for (size_t threadIndex = 0; threadIndex < amountThreads; ++threadIndex)
			{
				size_t subpartOffset = threadIndex * sizeParts;
				size_t subpartSize = std::min(sizeParts, _sourceRGB.size - subpartOffset);

				RGBAImageSSE& sourceSubpart = _threadPool[threadIndex].getSource();
				sourceSubpart.pixels = _sourceRGB.pixels + subpartOffset;
				sourceSubpart.size = subpartSize;

				YUVImage& destSubpart = _threadPool[threadIndex].getDestination();
				destSubpart.y = _resultYUV.y + subpartOffset;
				destSubpart.u = _resultYUV.u + subpartOffset;
				destSubpart.v = _resultYUV.v + subpartOffset;
				destSubpart.size = subpartSize;
			}

			{
				std::unique_lock<std::mutex> lock(_mutex);

				// Unlock the worker threads so that they can start running their task.
				_valueSyncVariable = UnlockWorkerThreadsValue;
				_syncVariable.notify_all();
			}

			{
				std::unique_lock<std::mutex> lock(_mutex);

				// Wait all threads are running.
				_syncVariable.wait(lock, [&]() { return _runningThreads == amountThreads; });
				// Wait all threads have finished their task.
				_syncVariable.wait(lock, [&]() { return _completedThreads == amountThreads; });
			}
		}
		_jobDone = true;
	}
	else {
		for (int i = 0; i < MaxBenchLoops; ++i)
		{
			convert_rgb2yuv(_sourceRGB, &_resultYUV);
		}
	}

	return _resultYUV;
}


void RGB2YUVUtilities::conversion_task(size_t threadIndex)
{
	while (!_jobDone)
	{
		{
			std::unique_lock<std::mutex> lock(_mutex);

			//Synchronize the start of the threads.
			_syncVariable.wait(lock, [&]() { return _valueSyncVariable == UnlockWorkerThreadsValue; });

			_runningThreads++;
			if (_runningThreads == _threadPool.size())
			{
				_completedThreads = 0;
			}

			_syncVariable.notify_all();
		}

		{
			std::unique_lock<std::mutex> lock(_mutex);

			// When the last thread to be ready is here, it can notify
			// all the waiting worker threads to run their conversion task.
			_syncVariable.wait(lock, [&]() { return _runningThreads == _threadPool.size(); });
			_syncVariable.notify_all();
		}

		// Run the conversion itself.
		convert_rgb2yuv
		(
			_threadPool[threadIndex].getSource(),
			&_threadPool[threadIndex].getDestination()
		);

		{
			std::unique_lock<std::mutex> lock(_mutex);
		
			// Synchronize the end of the tasks.
			_completedThreads++;
			if (_completedThreads == _threadPool.size()) {
				_valueSyncVariable = LockWorkerThreadsValue;
				_runningThreads = 0;
			}

			// Make sure all threads finish their task and wait the main thread
			// to set up data for the next round.
			_syncVariable.wait(lock, [&]() { return _completedThreads == _threadPool.size(); });
			_syncVariable.notify_all();
		}
	}
}