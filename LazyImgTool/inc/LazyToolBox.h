#ifndef _INC_TOOL_BOX_H_
#define _INC_TOOL_BOX_H_

#ifndef NOT_NEED_COLOR_TOOLS

#ifndef USE_CUDA
#include "LazyImgCudaModConfig.h"
#else

#endif // USE_CUDA IS END

namespace lazyimg
{
	class ToolBox
	{
	public:
#ifndef USE_CUDA
		// get gpu struction
		NVDAGpuIdx device_setting;
		ToolBox()
		{
			GetGpuSetting(0, &device_setting);
		}
		~ToolBox()
		{
			UnlinkGpuSetting();
		}
#else

#endif // USE_CUDA IS END
	private:
		// std::vector<Mat*> cpu_ram_stream;
		// std::vector<GMat> gpu_ram_stream;
	};
}

#endif // NOT_NEED_COLOR_TOOLS IS END

#endif //_INC_TOOL_BOX_H_ IS EOF