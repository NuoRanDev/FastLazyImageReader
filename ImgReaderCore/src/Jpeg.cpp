#include "LazyImgType.h"
#include <jpeglib.h>
#include "LazyCore.h"

bool ReadJpeg(lazyimg::Mat* output, FILE* file_ptr)
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, file_ptr);
	//read header
	jpeg_read_header(&cinfo, true);
	//start decompress
	jpeg_start_decompress(&cinfo);

	// line size
	uint32_t stride = cinfo.output_width * cinfo.output_components;

	// read parameters
	switch (cinfo.out_color_space)
	{
	case JCS_UNKNOWN:
		printf("unkown jpeg type\n");
		return false;

	case JCS_GRAYSCALE:
		output->Create(lazyimg::DataType(pformat::U8, pformat::C1), cinfo.image_width, cinfo.image_height);
		output->color_type = pformat::U8_GRAYSCALE;
		break;

	case JCS_RGB:
		output->Create(lazyimg::DataType(pformat::U8, pformat::C3), cinfo.image_width, cinfo.image_height);
		output->color_type = pformat::U8_RGB;
		break;

	default:
		printf("unkown jpeg type\n");
		return false;
	}

	// get pixel lines
	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, output->GetLineStartPtr<lazyimg::byte>(cinfo.output_scanline), 1);
	}

	//finnished
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return true;
}

#ifndef READ_ONLY
#include "LazyToolFunction.h"
bool WriteJpeg(lazyimg::Mat* img, const char* path, uint32_t quality)
{
	FILE* file_ptr;
	// OPEN FILE
	file_ptr = fopen(path, "wb");
	if (file_ptr == nullptr)
	{
		printf("CAN'T WRITE FILE");
		return false;
	}
	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);

	// create compress
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, file_ptr);

	cinfo.image_height = img->y;
	cinfo.image_width = img->x;
	// save RGB
	switch (img->color_type)
	{
	case pformat::U8_RGBA:
#if defined(USE_CUDA)
		lazyimg::IMGRGBA2RGB(img, img, ExternDeviceSetting);
#else
		lazyimg::IMGRGBA2RGB(img, img);
#endif
		break;
	case pformat::U8_RGB:
		break;
	default:
		goto FREE;
	}
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);

	// start compress
	jpeg_start_compress(&cinfo, true);

	while (cinfo.next_scanline < cinfo.image_height) {
		jpeg_write_scanlines(&cinfo, img->GetLineStartPtr<lazyimg::byte>(cinfo.next_scanline), 1);
	}
	jpeg_finish_compress(&cinfo);

	FREE:
	// free
	jpeg_destroy_compress(&cinfo);
	fclose(file_ptr);

	return true;
}
#endif //READ_ONLY IS END