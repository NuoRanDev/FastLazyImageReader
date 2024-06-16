#ifndef _INC_LAZY_IMG_CUDA_MOD_CONFIG_H_
#define _INC_LAZY_IMG_CUDA_MOD_CONFIG_H_

#include "LazyImgConfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// NVDA GPU
struct NVDAGpuIdx
{
	uint32_t gpuidx;
	int32_t threads_from_block;
	uint32_t block_x;
	uint32_t block_y;
	uint32_t block_z;
};

// IMAGES STRUCTION
struct IMAGES_DATA_SEGMENT
{
	// GPU
	int32_t MAX_BLOCK_HAS_THREAD_NUMBER;
	// pixel number
	uint32_t all_image_pixel_number;
	// width
	uint32_t x;
	// hieght
	uint32_t y;
	// Data Need Block
	int32_t all_data_need_block;
	// Pixels need block
	int32_t all_pixel_need_block;
	//
	uint16_t channel;
	//
	uint16_t bit_depth;
};

// Get Gpu
bool LAZY_API_EXPORT GetGpuSetting(uint32_t index, NVDAGpuIdx* setting);

bool LAZY_API_EXPORT UnlinkGpuSetting();

#endif // _INC_LAZY_IMG_CUDA_MOD_CONFIG_H_ IS EOF