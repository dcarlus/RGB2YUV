#pragma once

#include <string>
#include <vector>
#include <mutex>

#include <ConversionThreadData.hpp>

extern "C" {
	#include <image.h>
	#include <yuv_converter.h>
}

/// <summary>
/// Convert a RGB picture to YUV.
/// </summary>
class RGB2YUVUtilities
{
	private:
		/// <summary>
		/// Value to lock the worker threads with the Synchronizer.
		/// </summary>
		const int LockWorkerThreadsValue = -1;

		/// <summary>
		/// Value to unlock the worker threads with the Synchronizer.
		/// </summary>
		const int UnlockWorkerThreadsValue = -2;


		/// <summary>
		/// Flag to know if the job is done.
		/// </summary>
		bool _jobDone = false;

		/// <summary>
		/// Width of the picture.
		/// </summary>
		int _width = 0;

		/// <summary>
		/// Height of the picture.
		/// </summary>
		int _height = 0;

		/// <summary>
		/// Amount of pixels (cache).
		/// </summary>
		int _amountPixels = 0;

		/// <summary>
		/// The RGB source picture.
		/// </summary>
		RGBAImageSSE _sourceRGB;

		/// <summary>
		/// Resulting YUV picture.
		/// </summary>
		YUVImage _resultYUV;

		/// <summary>
		/// Pool of threads to run parallel tasks of
		/// conversion on a same picture.
		/// As thread creation takes a lot of time, it
		/// is really interesting to reuse them over time.
		/// </summary>
		std::vector<ConversionThreadData> _threadPool;

		/// <summary>
		/// Amount of threads running.
		/// </summary>
		int _runningThreads = 0;

		/// <summary>
		/// Amount of threads having completed their task.
		/// </summary>
		int _completedThreads = 0;

		/// <summary>
		/// Prevent concurrent accesses to inner data.
		/// </summary>
		std::mutex _mutex;

		/// <summary>
		/// Value of the variable to synchronize several threads.
		/// Each thread wait for it to have a specific value to continue
		/// its execution.
		/// </summary>
		int _valueSyncVariable = 0;

		/// <summary>
		/// Used to synchronize the threads in the pool.
		/// They must start at the same time.
		/// </summary>
		std::condition_variable _syncVariable;


	public:
		/// <summary>
		/// Create a new RGB to YUV converter.
		/// </summary>
		/// <param name="amountThreads">
		/// Amount of threads to convert the picture(s).
		/// </param>
		RGB2YUVUtilities(const unsigned int amountThreads);

		/// <summary>
		/// Destruction of the converter.
		/// </summary>
		virtual ~RGB2YUVUtilities();

		/// <summary>
		/// Load a PPM file in RGB colorspace.
		/// </summary>
		/// <param name="rgbFilePath">
		/// RGB picture file to load in PPM file format.
		/// </param>
		void loadPPM(const std::string& rgbFilePath);

		/// <summary>
		/// Initialize the thread pool.
		/// </summary>
		void initializeThreads();

		/// <summary>
		/// Convert the lastly loaded picture to YUV colorspace.
		/// </summary>
		/// <returns>
		/// The resulting YUV picture.
		/// </returns>
		YUVImage convert();

		/// <summary>
		/// Get the width of the converted picture.
		/// </summary>
		/// <returns>
		/// Width of the picture.
		/// </returns>
		int width() const
		{
			return _width;
		}

		/// <summary>
		/// Get the height of the converted picture.
		/// </summary>
		/// <returns>
		/// Height of the picture.
		/// </returns>
		int height() const
		{
			return _height;
		}


	private:
		/// <summary>
		/// Conversion task to run in parallel.
		/// </summary>
		/// <param name="threadIndex">
		/// Index of the thread to retreive its subpart
		/// of the RGBA picture to convert.
		/// </param>
		void conversion_task(size_t threadIndex);
};