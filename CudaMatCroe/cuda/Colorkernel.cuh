#ifndef _INC_CUDA_COLOR_KERNEL_CUH_
#define _INC_CUDA_COLOR_KERNEL_CUH_

#include "LazyImgCudaModConfig.h"

bool gpuXYZ2LAB	 (float* src,		float* dst,			IMAGES_DATA_SEGMENT gpu_setting);

bool gpuRGB2XYZ	 (uint8_t* src,		float* dst,			IMAGES_DATA_SEGMENT gpu_setting);

bool gpuRGBA2RGB (uint8_t* src,		uint8_t* dst,		IMAGES_DATA_SEGMENT gpu_setting);

#endif // _INC_CUDA_COLOR_KERNEL_CUH_ IS EOF