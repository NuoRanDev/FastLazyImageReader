#include "LazyImgType.h"
#include <jpeglib.h>

bool ReadJpeg(lazyimg::Mat* output, FILE* fie_ptr)
{
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, fie_ptr);
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

	JSAMPROW buffer[1] = { 0 };
	buffer[0] = output->at<lazyimg::byte>();

	// get pixel lines
	while (cinfo.output_scanline < cinfo.output_height)
	{
		buffer[0] = buffer[0] + stride;
		jpeg_read_scanlines(&cinfo, buffer, 1);
	}

	//finnished
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	return true;
}

#ifndef READ_ONLY
bool WriteJpeg(lazyimg::Mat img, const char* path, uint32_t quality)
{
	return true;
}
#endif //READ_ONLY IS END