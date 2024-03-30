#include "LazyImgio.h"

bool ReadImg(lazyimg::Mat* dst, std::string& path)
{
	FILE* file = fopen(path.c_str(), "rb");
	if (file == nullptr)
	{
		printf("no path\n");
		return false;
	}

	bool state = true;
	if (path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos)
	{
		state = ReadJpeg(dst, file);
		goto END;
	}
	if (path.find(".png") != std::string::npos)
	{
		state = ReadPng(dst, file);
		goto END;
	}
#ifdef ADD_TIFF_MODULES
	if (path.find(".tif") != std::string::npos || path.find(".tiff") != std::string::npos)
	{
		fclose(file);
		state = ReadTiff(dst, path.c_str());
		return state;
	}
#endif // ADD_TIFF_MODULES
	END:
	fclose(file);
	return state;
}

#ifndef READ_ONLY
bool WriteImg(lazyimg::Mat& src, std::string& path, uint32_t quality)
{
	bool state = true;
	if (path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos)
	{
		state = WriteJpeg(src, path.c_str());
	}
	if (path.find(".png") != std::string::npos)
	{
		state = WritePng(src, path.c_str());
	}
#ifdef ADD_TIFF_MODULES
	if (path.find(".tif") != std::string::npos || path.find(".tiff") != std::string::npos)
	{
		state = WriteTiff(src, path.c_str());
	}
#endif // ADD_TIFF_MODULES
	return state;
}
#endif // READ_ONLY IS END