#include "LazyImgio.h"

bool ReadTiff(lazyimg::Mat* output, const char* file_path, uint32_t n_frame)
{
	return false;
}

#ifndef READ_ONLY
bool WriteTiff(lazyimg::Mat img, const char* path, uint32_t n_frame)
{
	return false;
}
#endif //READ_ONLY IS END