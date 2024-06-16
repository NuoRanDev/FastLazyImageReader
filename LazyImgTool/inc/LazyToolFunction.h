#ifndef _INC_TOOL_FUNCTION_H_
#define _INC_TOOL_FUNCTION_H_
#include "LazyMat.h"

#if !defined(NOT_NEED_COLOR_TOOLS)

#include "LazyToolBox.h"

#ifdef USE_CUDA
#include "LazyGMat.h"
#endif // USE_CUDA IS END

namespace lazyimg
{
#if defined(USE_CUDA)
	bool IMGRGBA2RGB(Mat* src, Mat* dst, ToolBox device);
#else
	void IMGRGBA2RGB(Mat* src, Mat* dst);
#endif
}
#endif // NOT_NEED_COLOR_TOOLS IS END
namespace lazyimg
{
	bool BigTurnLittleEndian_8Bit(Mat* src);
}
#endif // _INC_TOOL_FUNCTION_H_ IS EOF