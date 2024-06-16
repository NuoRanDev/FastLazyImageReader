#include "LazyGMat.h"
#include "Kernel.hpp"
#include "Colorkernel.cuh"

lazyimg::GMat::GMat(NVDAGpuIdx i_gpu_setting)
{
	gpu_data_size = 0;
	x = 0, y = 0;
	channel = 0;
	bit_depth = 0;
	image_stream_setting.MAX_BLOCK_HAS_THREAD_NUMBER = i_gpu_setting.threads_from_block;
	// nullptr is empty
	pG_data = nullptr;
}

bool lazyimg::GMat::LoadDataInGPU(Mat* src)
{
	bool state = true;
	state = Create(src->x, src->y, src->channel, src->bit_depth);
	if(!state)
	{
		printf("Malloc In GPU Falided\n");
		return state;
	}
	state = CopyDataCPUToGPU(pG_data, src->at<byte>(), gpu_data_size);
	return state;
}

bool lazyimg::GMat::Create(uint32_t i_x, uint32_t i_y, byte i_channel, byte i_bit_depth)
{
	bool state = true;
	x = i_x;
	y = i_y;
	bit_depth = i_bit_depth;
	channel = i_channel;
	gpu_data_size = x * y * bit_depth * channel;

	image_stream_setting.x = x;
	image_stream_setting.y = y;
	image_stream_setting.all_data_need_block = (uint32_t)(gpu_data_size / image_stream_setting.MAX_BLOCK_HAS_THREAD_NUMBER) + 1;
	image_stream_setting.channel = channel;
	image_stream_setting.all_pixel_need_block = (uint32_t)(gpu_data_size / channel / image_stream_setting.MAX_BLOCK_HAS_THREAD_NUMBER) + 1;
	image_stream_setting.all_image_pixel_number = x * y;
	image_stream_setting.bit_depth = bit_depth;
	// CREATE 
	pG_data = CreateMemoryInGPU<byte>(gpu_data_size);
	if (pG_data == nullptr)
	{
		return false;
	}
	return state;
}

bool lazyimg::GMat::empty()
{
	// nullptr is empty
	return pG_data == nullptr;
}

bool lazyimg::GMat::CopyInCpu(Mat* src)
{
	bool state = CopyDataGPUToCPU<byte>(src->at<byte>(), pG_data, gpu_data_size);
	return state;
}

void lazyimg::GMat::Release()
{
	if (pG_data != nullptr)
	{
		cudaFree(pG_data);
		pG_data = nullptr;
	}
}

bool lazyimg::RGB2XYZ(GMat* src, GMat* dst)
{
	bool state = true;
	uint32_t dst_bit_depth = sizeof(float);
	state = dst->Create(src->x, src->y, src->channel, dst_bit_depth * 3);
	if (!state) return state;
	state = gpuRGB2XYZ(src->at<byte>(), dst->at<float>(), src->image_stream_setting);
	if (!state) return state;
	return true;
}

bool lazyimg::XYZ2LAB(GMat* src, GMat* dst)
{
	bool state = true;
	uint32_t dst_bit_depth = sizeof(float);
	state = dst->Create(src->x, src->y, src->channel, dst_bit_depth * 3);
	if (!state) return state;
	state = gpuXYZ2LAB(src->at<float>(), dst->at<float>(), src->image_stream_setting);
	if (!state) return state;
	return true;
}

bool lazyimg::RGBA2RGB(GMat* src, GMat* dst)
{
	bool state = true;
	uint32_t dst_bit_depth = sizeof(byte);
	state = dst->Create(src->x, src->y, 3u, dst_bit_depth);
	if (!state) return state;
	state = gpuRGBA2RGB(src->at<byte>(), dst->at<byte>(), src->image_stream_setting);
	if (!state) return state;
	return true;
}