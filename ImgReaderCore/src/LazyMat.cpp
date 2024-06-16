#include "LazyMat.h"

#include "LazyImgConfig.h"
#include <iostream>
#include "mimalloc.h"

namespace lazyimg
{
	void Mat::GengralInit(byte type)
	{
		// init channel
		byte _type_channel = type & 0b0000'1111;

		switch (_type_channel)
		{
		case pformat::C1:
			channel = 1;
			break;

		case pformat::C2:
			channel = 2;
			break;

		case pformat::C3:
			channel = 3;
			break;

		case pformat::C4:
			channel = 4;
			break;
		}

		// init size
		byte _type_pix_size = (type & 0b1111'0000) >> 4;
		switch (_type_pix_size)
		{
		case pformat::U8:
			bit_depth = 1;
			break;

		case pformat::U16:
			bit_depth = 2;
			break;

		case pformat::U24:
			bit_depth = 3;
			break;

		case pformat::U32:
			bit_depth = 4;
			break;
		}
	}

	void Mat::FindStartPtr()
	{
		ALLOC_PIX_DATA:
		start_line_ptr = (void**)mi_malloc(y * sizeof(void*));
		if (start_line_ptr == nullptr)goto ALLOC_PIX_DATA;
		
		for (uint32_t i = 0; i < y; i++)
		{
			start_line_ptr[i] = ppix_data + i * x * channel * bit_depth;
		}
	}

	Mat::Mat(byte type, uint32_t w, uint32_t h) :ppix_data(0)
	{
		GengralInit(type);
		x = w, y = h;
		_this_mat_size = (size_t)x * y * channel * bit_depth;

		// alloc data
		ALLOC_PIX_DATA:
		ppix_data = (byte*)mi_malloc(_this_mat_size);
		if (ppix_data == nullptr)goto ALLOC_PIX_DATA;

#ifdef _DEBUG
		memset(ppix_data, 0, _this_mat_size);
		memcpy(ppix_data, "this is debug", 14l);
		std::cout << "START" << (void*)ppix_data << "\t TO \t END:" << (void*)(ppix_data + _this_mat_size) << std::endl;
#endif // _DEBUG
		FindStartPtr();
	}

	Mat::Mat(byte type, uint32_t w, uint32_t h, const void* i_pdata) :ppix_data(0)
	{
		GengralInit(type);
		x = w, h = y;

		_this_mat_size = (size_t)x * y * channel * bit_depth;
		// alloc data
		ALLOC_PIX_DATA:
		ppix_data = (byte*)mi_malloc(_this_mat_size);
		if (ppix_data == nullptr)goto ALLOC_PIX_DATA;

		// copy data
		memcpy(ppix_data, i_pdata, _this_mat_size);
		FindStartPtr();
	}

	Mat::Mat(Mat& src)
	{
		x = src.x;
		y = src.y;
		color_type = src.color_type;
		channel = src.channel;
		bit_depth = src.bit_depth;
		ppix_data = src.ppix_data;
		_this_mat_size = src._this_mat_size;
		start_line_ptr = src.start_line_ptr;
	}

	void Mat::Release()
	{
		if (ppix_data != nullptr)
		{
			mi_free(ppix_data);
			ppix_data = nullptr;
		}
		if (ppix_data != nullptr)
		{
			mi_free(start_line_ptr);
			start_line_ptr = nullptr;
		}
		x = 0;
		y = 0;
		color_type = 0;
		channel = 0;
		bit_depth = 0;
		_this_mat_size = 0;
	}

	void Mat::ByteBlockCopy(byte* pByteBlock, size_t _size)
	{
		memcpy(pByteBlock, ppix_data, _size);
	}

	void Mat::Create(byte type, uint32_t w, uint32_t h)
	{
		Release();
		GengralInit(type);
		x = w, y = h;
		_this_mat_size = (size_t)x * y * channel * bit_depth;

		// alloc data
	ALLOC_PIX_DATA:
		ppix_data = (byte*)mi_malloc(_this_mat_size);
		if (ppix_data == nullptr)goto ALLOC_PIX_DATA;

#ifdef _DEBUG
		memset(ppix_data, 0, _this_mat_size);
		memcpy(ppix_data, "this is debug", 14l);
		std::cout << "START" << (void*)ppix_data << "\t TO \t END:" << (void*)(ppix_data + _this_mat_size) << std::endl;
#endif // _DEBUG
		FindStartPtr();
	}

	void Mat::Create(byte type, uint32_t w, uint32_t h, const void* i_pdata)
	{
		Release();
		GengralInit(type);
		x = w, y = h;

		_this_mat_size = (size_t)x * y * channel * bit_depth;
		// alloc data
	ALLOC_PIX_DATA:
		ppix_data = (byte*)mi_malloc(_this_mat_size);
		if (ppix_data == nullptr)goto ALLOC_PIX_DATA;

		// copy data
		memcpy(ppix_data, i_pdata, _this_mat_size);
		FindStartPtr();
	}

	void Mat::Create(byte i_channel, byte i_bit_depth, uint32_t w, uint32_t h, const void* i_pdata)
	{
		CreateNotInitEmpty(i_channel, i_bit_depth, w, h);
		memcpy(ppix_data, i_pdata, _this_mat_size);
	}

	void Mat::CreateNotInitEmpty(byte i_channel, byte i_bit_depth, uint32_t w, uint32_t h)
	{
		Release();
		x = w;
		y = h;
		channel = i_channel;
		bit_depth = i_bit_depth;
		_this_mat_size = (size_t)w * h * i_channel * i_bit_depth;
		// alloc data
	ALLOC_PIX_DATA:
		ppix_data = (byte*)mi_malloc(_this_mat_size);
		if (ppix_data == nullptr)goto ALLOC_PIX_DATA;
		FindStartPtr();
	}

	void Mat::CloneTo(Mat* dst)
	{
		dst->Create(channel, bit_depth, x, y, ppix_data);
		dst->color_type = color_type;
	}

	Mat::~Mat()
	{
		Release();
	}
}