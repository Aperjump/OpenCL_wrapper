/*
 *	Platform test code 
 */

#if defined __APPLE__ || defined(MACOSX)
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

#include <iostream>
#include <vector>
#include <CL\cl.h>

int main()
{
	cl_platform_id* platforms;
	cl_uint num_platforms;
	cl_int err;
	err = clGetPlatformIDs(1, NULL, &num_platforms);
	if (err < 0) {
		perror("Couldn't find any platform\n");
		exit(1);
	}
	platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * num_platforms);
	clGetPlatformIDs(num_platforms, platforms, NULL);
	for (cl_uint i = 0; i < num_platforms; i++)
	{
		size_t ext_size;
		char* ext_data;
		err = clGetPlatformInfo(platforms[i],
			CL_PLATFORM_NAME, 0, NULL, &ext_size);
		ext_data = (char*)malloc(ext_size);
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, ext_size, ext_data, NULL);
		printf("Platform vendor is %s\n", ext_data);
		cl_device_id* devices;
		cl_uint num_device;
		err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1, NULL, &num_device);
		devices = (cl_device_id*)malloc(sizeof(cl_device_id) * num_device);
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, num_device, devices, NULL);
		char name_data[48];
		cl_ulong size;
		cl_uint cache_line;
		for (cl_uint j = 0; j < num_device; j++)
		{
			err = clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(name_data), name_data, NULL);
			printf("device name is %s\n", name_data);
			err = clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(size), &size, NULL);
			printf("device global cache size is %lld\n", size);
			err = clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cache_line), &cache_line, NULL);
			printf("device global cache line is %d\n", cache_line);
			err = clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(size), &size, NULL);
			printf("device global memory size is %lld\n", size);
			err = clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(size), &size, NULL);
			printf("device local memory size is %lld\n", size);
		}
		free(ext_data);
	}
	return 0;
}
/*
Platform vendor is NVIDIA CUDA
device name is GeForce GTX 1060
device global cache size is 163840
device global cache line is 128
device global memory size is 6442450944
device local memory size is 49152
Platform vendor is Intel(R) OpenCL
device name is Intel(R) HD Graphics 630
device global cache size is 524288
device global cache line is 64
device global memory size is 1561123226
device local memory size is 65536
*/