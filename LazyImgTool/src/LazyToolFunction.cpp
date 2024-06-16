#include "LazyToolBox.h"
#include "LazyToolFunction.h"

#if defined(USE_CUDA)
bool lazyimg::IMGRGBA2RGB(Mat* src, Mat* dst, ToolBox device)
{
	bool state;
	GMat* g_src = new GMat(device.device_setting);
	GMat* g_dst = new GMat(device.device_setting);

	state = g_src->LoadDataInGPU(src);

	RGBA2RGB(g_src, g_dst);

	dst->Create(lazyimg::DataType(pformat::U8, pformat::C3), src->x, src->y);
	g_dst->CopyInCpu(dst);
	dst->color_type = pformat::U8_RGB;

	delete g_src;
	delete g_dst;

	return state;
}
#else
void lazyimg::IMGRGBA2RGB(Mat* src, Mat* dst)
{
	dst->Create(lazyimg::DataType(pformat::U8, pformat::C3), src->x, src->y);
	byte* psrc = src->at<byte>();
	byte* pdst = dst->at<byte>();
	for (size_t i = 0; i < (size_t)src->x * src->y; i++)
	{
		pdst[i * 3 + 1] = psrc[i * 4 + 1];
		pdst[i * 3 + 2] = psrc[i * 4 + 2];
		pdst[i * 3 + 3] = psrc[i * 4 + 3];
	}
}
#endif // NOT_NEED_COLOR_TOOLS IS END


bool lazyimg::BigTurnLittleEndian_8Bit(Mat* src)
{
	uint32_t channel = 0;
	// Setting color type
	switch (src->color_type)
	{
	case pformat::U8_ABGR:
		channel = 4;
		src->color_type = pformat::U8_RGBA;
		break;
	case pformat::U8_RGBA:
		channel = 4;
		src->color_type = pformat::U8_ABGR;
		break;
	case pformat::U8_BGR:
		channel = 3;
		src->color_type = pformat::U8_RGB;
		break;
	case pformat::U8_RGB:
		channel = 3;
		src->color_type = pformat::U8_BGR;
		break;
	default:
		printf("Not input true image type\n");
		return false;
	}
	byte* ptr = src->at<byte>();
	for (size_t j = 0; j < (src->DataSize() / channel); j++)
	{
		for (uint32_t i = 0; i < (uint32_t)(channel / 2); i++)
		{
			byte mod = ptr[j * channel + channel - i - 1];
			ptr[j * channel + channel - i - 1] = ptr[j * channel + i];
			ptr[j * channel + i] = mod;
		}
	}
	return true;
}
