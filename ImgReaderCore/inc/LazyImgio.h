#ifndef _INC_LAZY_IMG_IO_H_
#define _INC_LAZY_IMG_IO_H_

#include "LazyMat.h"
#include "LazyImgType.h"
#include "LazyImgConfig.h"

// read image from image file
bool LAZY_API_EXPORT ReadImg(lazyimg::Mat* dst, std::string& path);

// write image to image file
bool LAZY_API_EXPORT WriteImg(lazyimg::Mat& src, std::string& path, uint32_t quality = 8);

#endif // _INC_LAZY_IMG_IO_H_ IS EOF