#include "Kernel.hpp"
#include "Colorkernel.cuh"

__global__ void RGB2XYZ_Stream_Kernel(uint8_t* src, float* dst, uint32_t pixnumber, int MAX_BLOCK_UNIT)
{
	uint32_t id_x = threadIdx.x + MAX_BLOCK_UNIT * blockIdx.x;

	if (id_x > pixnumber)
	{
		return;
	}
	// pixel offset
	uint32_t p = MAX_BLOCK_UNIT * blockIdx.x * id_x;

	// RGBA offset
	uint8_t* prgba_cur = src + (p * 3);

	// LAB offset
	float* plab_cur = dst + (p * 3);

	// R
	float RR = (float)prgba_cur[0] / 255.0f;
	RR = RR > 0.04045 ? powf((RR + 0.055f) / 1.055f, 2.4f) : (RR / 12.92);
	// G
	float GG = (float)prgba_cur[1] / 255.0f;
	GG = GG > 0.04045 ? powf((GG + 0.055f) / 1.055f, 2.4f) : (GG / 12.92);
	// B
	float BB = (float)prgba_cur[2] / 255.0f;
	BB = BB > 0.04045 ? powf((BB + 0.055f) / 1.055f, 2.4f) : (BB / 12.92);
	// X
	plab_cur[0] = 0.4124564f * RR + 0.3575761f * GG + 0.1804375f * BB;
	// Y
	plab_cur[1] = 0.2126729f * RR + 0.7151522f * GG + 0.0721750f * BB;
	// Z
	plab_cur[2] = 0.0193339f * RR + 0.1191920f * GG + 0.9503041f * BB;
}

__global__ void XYZ2LAB_Stream_Kernel(float* src, float* dst, uint32_t pixnumber, int MAX_BLOCK_UNIT)
{
	uint32_t id_x = threadIdx.x + MAX_BLOCK_UNIT * blockIdx.x;

	if (id_x > pixnumber)
	{
		return;
	}

	// pixel offset
	uint32_t p = blockIdx.x * MAX_BLOCK_UNIT + id_x;

	// XYZ offset
	float* pxyz_cur = src + (p * 3);

	// LAB offset
	float* plab_cur = dst + (p * 3);

	// XYZ -> LAB
	const float param_13 = 1.0f / 3.0f;
	const float param_16116 = 16.0f / 116.0f;
	const float Xn = 0.950456f;
	const float Yn = 1.0f;
	const float Zn = 1.088754f;

	float fX, fY, fZ;

	float X = pxyz_cur[0] / (Xn);
	float Y = pxyz_cur[1] / (Yn);
	float Z = pxyz_cur[2] / (Zn);

	if (Y > 0.008856f)
		fY = pow(Y, param_13);
	else
		fY = 7.787f * Y + param_16116;

	if (X > 0.008856f)
		fX = pow(X, param_13);
	else
		fX = 7.787f * X + param_16116;

	if (Z > 0.008856)
		fZ = pow(Z, param_13);
	else
		fZ = 7.787f * Z + param_16116;

	float L = 116.0f * fY - 16.0f;
	L = L > 0.0f ? L : 0.0f;
	float A = 500.0f * (fX - fY);
	float B = 200.0f * (fY - fZ);

	plab_cur[0] = L;
	plab_cur[1] = A;
	plab_cur[2] = B;
}

__global__ void RGBA2RGB_Stream_Kernel(uint8_t* src, uint8_t* dst, uint32_t pixnumber, int MAX_BLOCK_UNIT)
{
	// pixel offset
	uint32_t id_x = threadIdx.x + MAX_BLOCK_UNIT * blockIdx.x;

	if (id_x > pixnumber)
	{
		return;
	}

	// RGBA offset
	uint8_t* prgba_cur = src + (id_x * 4);
	// RGB offset
	uint8_t* prgb_cur = dst + (id_x * 3);
	
	// copy pixel 
	prgb_cur[0] = prgba_cur[0];
	prgb_cur[1] = prgba_cur[1];
	prgb_cur[2] = prgba_cur[2];
}

__global__ void RGB2RBGA_Stream_Kernel(uint8_t* src, uint8_t* dst, uint32_t pixnumber, int MAX_BLOCK_UNIT)
{
	// pixel offset
	uint32_t id_x = threadIdx.x + MAX_BLOCK_UNIT * blockIdx.x;

	if (id_x > pixnumber)
	{
		return;
	}

	// RGBA offset
	uint8_t* prgba_cur = src + (id_x * 3);
	// RGB offset
	uint8_t* prgb_cur = dst + (id_x * 4);

	// copy pixel 
	prgba_cur[0] = prgb_cur[0];
	prgba_cur[1] = prgb_cur[1];
	prgba_cur[2] = prgb_cur[2];
	prgba_cur[3] = UCHAR_MAX;
}

bool gpuXYZ2LAB(float* src, float* dst, IMAGES_DATA_SEGMENT gpu_setting)
{
	XYZ2LAB_Stream_Kernel <<< gpu_setting.all_pixel_need_block, gpu_setting.MAX_BLOCK_HAS_THREAD_NUMBER >>> (src, dst, gpu_setting.all_image_pixel_number, gpu_setting.MAX_BLOCK_HAS_THREAD_NUMBER);
	return GetKernelFuntionStatus();
}

bool gpuRGB2XYZ(uint8_t* src, float* dst, IMAGES_DATA_SEGMENT gpu_setting)
{
	RGB2XYZ_Stream_Kernel <<< gpu_setting.all_pixel_need_block, gpu_setting.MAX_BLOCK_HAS_THREAD_NUMBER >>> (src, dst, gpu_setting.all_image_pixel_number, gpu_setting.MAX_BLOCK_HAS_THREAD_NUMBER);
	return GetKernelFuntionStatus();
}

bool gpuRGBA2RGB(uint8_t* src, uint8_t* dst, IMAGES_DATA_SEGMENT gpu_setting)
{
	RGBA2RGB_Stream_Kernel <<< gpu_setting.all_pixel_need_block, gpu_setting.MAX_BLOCK_HAS_THREAD_NUMBER >>> (src, dst, gpu_setting.all_image_pixel_number, gpu_setting.MAX_BLOCK_HAS_THREAD_NUMBER);
	return GetKernelFuntionStatus();
}