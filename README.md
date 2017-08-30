# RGB2YUV
This repo contains a C library for converting RGB data to YUV.
It exports the YUV data into three separated channels (Y, U and V) so that they can be combined as wanted by the developer.

The C API is great to easily wrap the library for other languages.
It contains few public functions limiting the work to wrap it.

SSE4.1 is used to run the conversion as fast as possible.
The RGB data can easily be split to run the conversion in parallel with many threads.

A demo of the library use is provided as an executable.
