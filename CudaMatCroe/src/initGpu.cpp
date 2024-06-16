#include "LazyImgCudaModConfig.h"
#include "Kernel.hpp"

bool GetGpuSetting(uint32_t index, NVDAGpuIdx* setting)
{
	return InitCUDA(index, setting);
}

bool UnlinkGpuSetting()
{
	return DistoryCudaItem();
}