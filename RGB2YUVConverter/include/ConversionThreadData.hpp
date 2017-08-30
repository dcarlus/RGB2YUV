#pragma once

#include <thread>

extern "C" {
	#include <image.h>
	#include <yuv_converter.h>
}

class ConversionThreadData
{
	private:
		/// <summary>
		/// Subpart of the picture the thread will have to
		/// convert.
		/// </summary>
		RGBAImageSSE _source;

		/// <summary>
		/// Subpart of the picture in which the thread will
		/// store the converted pixels.
		/// </summary>
		YUVImage _destination;

	public:
		/// <summary>
		/// Set the part of the picture source for the task.
		/// </summary>
		/// <param name="source">
		/// Source picture part to convert in the task.
		/// </param>
		void setSource(RGBAImageSSE source)
		{
			_source = source;
		}

		/// <summary>
		/// Get the part of the picture source for the task.
		/// </summary>
		/// <returns>
		/// Source picture part to convert in the task.
		/// </returns>
		RGBAImageSSE& getSource()
		{
			return _source;
		}

		/// <summary>
		/// Set the destination part of the converted picture.
		/// </summary>
		/// <param name="destination">
		/// Part of the picture the thread will store converted
		/// pixels in.
		/// </param>
		void setDestination(const YUVImage& destination)
		{
			_destination = destination;
		}

		/// <summary>
		/// Get the destination part of the converted picture.
		/// </summary>
		/// <returns>
		/// Part of the picture the thread will store converted
		/// pixels in.
		/// </returns>
		YUVImage& getDestination()
		{
			return _destination;
		}
};