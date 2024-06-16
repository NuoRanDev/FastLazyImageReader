#ifndef _INC_CUDA_KERNEL_HPP_
#define _INC_CUDA_KERNEL_HPP_

// cuda
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

// config
#include "LazyImgCudaModConfig.h"

// cuda kernel
inline bool IsHasNVDAGpu()
{
	int count = 0;
	cudaGetDeviceCount(&count);
	// if has cout >= 0 return true
	return count != 0;
}

// init cuda
inline bool InitCUDA(int GPU_number, NVDAGpuIdx* setting)
{
	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop, GPU_number);
	
	// Print Device Information
#ifdef _DEBUG
	printf("GPU: %s is running\nclock %d Hz\n", prop.name, prop.clockRate);
	printf("max thread num: %d form \n", prop.maxThreadsPerBlock);
	printf("max grid: x: %d y: %d z: %d\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
#endif // _DEBUG
	
	// Export Device Information
	setting->threads_from_block = prop.maxThreadsPerBlock;
	setting->block_x = prop.maxGridSize[0];
	setting->block_y = prop.maxGridSize[1];
	setting->block_z = prop.maxGridSize[2];

	if (cudaSetDevice(GPU_number) != cudaSuccess)
	{
#ifdef _DEBUG
		printf("Flie:%s Line:%d FUNCTION cudakernel.cu -> Initcuda() ERROR :\n", __FILE__, __LINE__);
		abort();
#endif // _DEBUG IS END
		return false;
	}
	return true;
}

template<typename T> inline T* CreateMemoryInGPU(size_t size)
{
	T* ptr = nullptr;
	if (cudaMalloc((void**)(&ptr), size * sizeof(T)) != cudaSuccess)
	{
#ifdef _DEBUG
		printf("Flie:%s Line:%d Function cudakernel.cu -> CreateMemoryInGPU(size_t size, T* ptr) ERROR :cudaMalloc failed!\n", __FILE__, __LINE__);
		abort();
#endif // _DEBUG IS END
		return nullptr;
	}
#ifdef _DEBUG
	printf("alloc : %lld B\n T size:%lld\n number :%lld\n", size * sizeof(T), sizeof(T), size);
#endif // _DEBUG
	return ptr;
}

inline bool GetKernelFuntionStatus()
{
	cudaError_t cudaStatus = cudaGetLastError();
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
#ifdef _DEBUG
		printf("Flie:%s Line:%d \n Kernel Function ERROR: %s\n", __FILE__, __LINE__, cudaGetErrorString(cudaStatus));
		printf("\tERROR CODE :%d \t\n", cudaStatus);
		abort();
#endif // _DEBUG IS END
		return false;
	}
#ifdef _DEBUG
	puts("run success");
#endif // _DEBUG
	return true;
}

template<typename T> inline bool CopyDataCPUToGPU(T* dst, const T* src, size_t size)
{
	if (cudaMemcpy(dst, src, sizeof(T) * size, cudaMemcpyHostToDevice) != cudaSuccess)
	{
#ifdef _DEBUG
		printf("Flie:%s Line:%d FUNCTION -> CopyDataCPUToGPU(T* dst,const T* src, size_t size) ERROR :cudaMemcpy failed!\n", __FILE__, __LINE__);
		abort();
#endif // _DEBUG IS END
		return false;
	}
#ifdef _DEBUG
	printf("copy cpu->gpu : %lld B\n T size:%lld\n number :%lld\n", size * sizeof(T), sizeof(T), size);
#endif // _DEBUG
	return true;
}

template<typename T> inline bool CopyDataGPUToCPU(T* dst, const T* src, size_t size)
{
	if (cudaMemcpy(dst, src, sizeof(T) * size, cudaMemcpyDeviceToHost) != cudaSuccess)
	{
#ifdef _DEBUG
		printf("Flie:%s Line:%d FUNCTION -> CopyDataCPUToGPU(T* dst,const T* src, size_t size) ERROR :cudaMemcpy failed!\n", __FILE__, __LINE__);
		abort();
#endif // _DEBUG IS END
		return false;
	}
#ifdef _DEBUG
	printf("copy gpu->cpu : %lld B\n T size:%lld\n number :%lld\n", size * sizeof(T), sizeof(T), size);
#endif // _DEBUG
	return true;
}

inline bool DistoryCudaItem()
{
	if (cudaDeviceReset() != cudaSuccess) {
#ifdef _DEBUG
		printf("Flie:%s Line:%d FUNCTION -> DistoryCudaItem() ERROR :cudaDeviceReset failed!\n", __FILE__, __LINE__);
		abort();
#endif // _DEBUG IS END
		return false;
	}
	return false;
}

template<typename T> inline bool MoveDataGPUToCPU(T* dst, const T* src, size_t size)
{
	bool state = CopyDataGPUToCPU(dst, src, size);
	cudaFree((void*)src);
	src = nullptr;
#ifdef _DEBUG
	printf("move gpu->cpu : %lld B\n T size:%lld\n number :%lld\n", size * sizeof(T), sizeof(T), size);
#endif // _DEBUG
	return state;
}

#endif // _INC_CUDA_KERNEL_HPP_  IS EOF