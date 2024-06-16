#include "LazyImgio.h"
#include "LazyToolFunction.h"
#include <png.h>

constexpr auto PNG_BYTES_TO_CHECK = 4;

bool ReadPng(lazyimg::Mat* output, FILE* file_ptr)
{
	png_structp png_ptr = nullptr;
	png_infop info_ptr = nullptr;

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr)
	{
		printf("png_ptr init false\n");
		return false;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		printf("info_ptr init false\n");
		return false;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		printf("Not png file\n");
		return false;
	}

#ifndef NOT_USE_C_STD_STREAM
	png_init_io(png_ptr, file_ptr);
#else
	// 后期添加内存映射和网络流
	png_set_read_fn(png_ptr, (void*)user_io_ptr, user_read_fn);
#endif
	//png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	// get data frame
	png_uint_32 img_width = png_get_image_width(png_ptr, info_ptr);
	png_uint_32 img_height = png_get_image_height(png_ptr, info_ptr);
	png_uint_32 bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	png_uint_32 img_channels = png_get_channels(png_ptr, info_ptr);
	png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);
	
	
	// get ALL pixel png is Big-endian 
	switch (color_type)
	{
	case PNG_COLOR_TYPE_RGB_ALPHA:
		if (bit_depth == 8)
		{
			output->Create(lazyimg::DataType(pformat::U8, pformat::C4), img_width, img_height);
			output->color_type = pformat::U8_ABGR;
			break;
		}
		if (bit_depth == 16)
		{
			png_set_strip_16(png_ptr);
			output->Create(lazyimg::DataType(pformat::U16, pformat::C4), img_width, img_height);
			output->color_type = pformat::U16_ABGR;
			break;
		}
		else
		{
			printf("Can\'t support this bit depth");
			return false;
		}

	case PNG_COLOR_TYPE_RGB:
		if (bit_depth == 8)
		{
			output->Create(lazyimg::DataType(pformat::U8, pformat::C3), img_width, img_height);
			output->color_type = pformat::U8_BGR;
			break;
		}
		if (bit_depth == 16)
		{
			png_set_strip_16(png_ptr);
			output->Create(lazyimg::DataType(pformat::U16, pformat::C3), img_width, img_height);
			output->color_type = pformat::U16_BGR;
			break;
		}
		else
		{
			printf("Can\'t support this bit depth");
			return false;
		}

	case PNG_COLOR_TYPE_GRAY:
		if (bit_depth <= 8)
		{
			if (bit_depth < 8)
			{
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}
			output->Create(lazyimg::DataType(pformat::U8, pformat::C1), img_width, img_height);
			output->color_type = pformat::U8_GRAYSCALE;
			break;
		}
		if (bit_depth == 16)
		{
			png_set_strip_16(png_ptr);
			output->Create(lazyimg::DataType(pformat::U16, pformat::C1), img_width, img_height);
			output->color_type = pformat::U16_GRAYSCALE;
			break;
		}
		else
		{
			printf("Can\'t support this bit depth");
			return false;
		}
	default:
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		printf("Can\'t support this format\n");
		return false;
	}
	png_read_image(png_ptr, output->AllGetLineStartPtr<uint8_t>());
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	lazyimg::BigTurnLittleEndian_8Bit(output);
	return true;
}

#if READ_ONLY
bool WritePng(lazyimg::Mat *img, const char* path)
{
	return false;
}
#endif //READ_ONLY IS END