#include "LazyImgio.h"

bool ReadTiff(lazyimg::Mat* output, const char* file_path, uint32_t n_frame)
{
	return false;
}

#if !defined(READ_ONLY) || !defined(NOT_NEED_COLOR_TOOLS)
bool WriteTiff(lazyimg::Mat* img, const char* path, uint32_t n_frame)
{
	return false;
}
#endif //defined(READ_ONLY) || defined(NOT_NEED_COLOR_TOOLS) IS END