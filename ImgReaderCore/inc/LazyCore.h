#ifndef _INC_LAZY_CORE_H_
#define _INC_LAZY_CORE_H_

// This tell us GPU  device setting
#if (defined(USE_CUDA)) || (!defined(READ_ONLY))
#include "LazyToolBox.h"
lazyimg::ToolBox ExternDeviceSetting = lazyimg::ToolBox();
#endif // READ_ONLY IS END


#endif // _INC_LAZY_CORE_H_ IS EOF