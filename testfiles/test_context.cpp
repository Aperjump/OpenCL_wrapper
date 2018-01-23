#include <cstdio>
#include <cstdlib>
#include <CL/opencl.h>

int try_closure(cl_context& context, cl_device_id device)
{
	cl_int err;
	cl_int refe_count;
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(refe_count), &refe_count, NULL);
	printf("reference counter is %d\n", refe_count);
	clRetainContext(context); // Comment this line will cause segmentation fault
	return 1;
}
int main()
{
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_int err;
	cl_int refe_count;
	err = clGetPlatformIDs(1, &platform, NULL);
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (err == CL_DEVICE_NOT_FOUND) {
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	}
	try_closure(context, device);
	err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(refe_count), &refe_count, NULL);
	printf("reference counter is %d\n", refe_count);
	clReleaseContext(context);
	err = clGetContextInfo(context, CL_CONTEXT_REFERENCE_COUNT, sizeof(refe_count), &refe_count, NULL);
	printf("reference counter is %d\n", refe_count);
}