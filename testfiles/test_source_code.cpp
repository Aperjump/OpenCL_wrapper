/*
This pieces of code cannot work since I do not pass kernel arguments.
*/
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <CL/cl.h>

int func3()
{
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_int err;
	cl_program program;
	FILE* program_handle;
	char* program_buffer;
	size_t program_size;

	size_t log_size;
	char* program_log;
	err = clGetPlatformIDs(1, &platform, NULL);
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (err == CL_DEVICE_NOT_FOUND) {
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	}

	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	program_handle = fopen("compute.cl", "r");
	if (program_handle == NULL) {
		perror("Couldn't find the program\n");
		exit(1);
	}

	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);
	program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, &err);
	if (err < 0) {
		perror("Couldn't build program\n");
		exit(1);
	}
	free(program_buffer);
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err < 0) {
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		program_log = (char*)malloc(log_size);
		program_log[log_size] = '\0';
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size + 1, program_log, NULL);
		printf("%s\n", program_log);
		free(program_log);
		exit(1);
	}

	cl_uint num_kernel;
	cl_kernel* kernels;
	char kernel_name[20];
	err = clCreateKernelsInProgram(program, 0, NULL, &num_kernel);
	kernels = (cl_kernel*)malloc(num_kernel * sizeof(cl_kernel));
	err = clCreateKernelsInProgram(program, num_kernel, kernels, NULL);
	if (err < 0) {
		perror("Create kernel error\n");
		exit(1);
	}
	for (cl_uint i = 0; i < num_kernel; i++)
	{
		clGetKernelInfo(kernels[i], CL_KERNEL_FUNCTION_NAME, sizeof(kernel_name), kernel_name, NULL);
		printf("kernel name is %s\n", kernel_name);
	}
	cl_command_queue queue;
	queue = clCreateCommandQueue(context, device, 0, &err);
	if (err < 0) {
		perror("Couldn't create the command queue\n");
		exit(1);
	}
	err = clEnqueueTask(queue, kernels[0], 0, NULL, NULL);
	if (err < 0) {
		perror("Couldn't enqueue kernel\n");
		exit(1);
	}
	else {
		printf("Successfully queued kernel\n");
	}
	for (cl_uint i = 0; i < num_kernel; i++)
		clReleaseKernel(kernels[i]);
	free(program_buffer);
	clReleaseProgram(program);
	clReleaseContext(context);
	return 1;
}