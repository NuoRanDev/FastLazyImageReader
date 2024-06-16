#ifndef _INC_IMG_COLOR_FORMAT_H_
#define _INC_IMG_COLOR_FORMAT_H_

#include "LazyMat.h"
#include "LazyImgCudaModConfig.h"

namespace lazyimg
{
	class LAZY_API_EXPORT GMat
	{
	private:
		// GPU DATA
		byte* pG_data;
		
	public:
		// gpu setting
		IMAGES_DATA_SEGMENT image_stream_setting;
		// width &heigth
		uint32_t x, y;
		// color channel
		byte channel;
		// pixel size
		byte bit_depth;
		// pixel data size
		size_t gpu_data_size;
		// init is empty
		GMat(NVDAGpuIdx i_gpu_setting);
		// copy
		GMat(const GMat &src)
		{
			x = src.x;
			y = src.y;
			channel = src.channel;
			bit_depth = src.bit_depth;
			gpu_data_size = src.gpu_data_size;
			image_stream_setting = src.image_stream_setting;
			pG_data = src.pG_data;
		}
		// losd
		bool LoadDataInGPU(Mat* src);
		// Create
		bool Create(uint32_t i_x, uint32_t i_y, byte i_channel, byte bit_depth);
		// is empty
		bool empty();
		//Get pixel data
		template<typename T> T* at()
		{
			return (T*)pG_data;
		}
		// Move Outgpu
		bool CopyInCpu(Mat* src);
		// clear data
		void Release();
		~GMat()
		{
			Release();
		}
	};

	// color tools
	bool LAZY_API_EXPORT RGB2XYZ(GMat* src, GMat* dst);
	bool LAZY_API_EXPORT XYZ2LAB(GMat* src, GMat* dst);
	bool LAZY_API_EXPORT RGBA2RGB(GMat* src, GMat* dst);
}

#endif // _INC_IMG_COLOR_FORMAT_H_ IS EOF