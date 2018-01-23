#pragma once
#include <CL/cl.h>

#include <cstdio>
#include <cstdlib>

cl_device_id new_device()
{
	cl_platform_id platform;
	cl_device_id device;
	cl_uint err;
	
	err = clGetPlatformIDs(1, &platform, NULL);
	if (err < 0) {
		perror("Cannot find a platform\n");
		exit(1);
	}
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (err == CL_DEVICE_NOT_FOUND) {
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	}
	if (err < 0) {
		perror("Cannot find a device\n");
	}
	return device;
}

size_t readfile(char* buffer, const char* filename)
{
	// should test whether file close successfully.
	FILE* file_handle;
	size_t program_size;
	
	file_handle = fopen(filename, "r");
	program_size = ftell(file_handle);
	rewind(file_handle);
	buffer = (char*)malloc(program_size + 1);
	buffer[program_size + 1] = '\0';
	fread(buffer, sizeof(char), program_size, file_handle);
	fclose(file_handle);
	return program_size;
}

cl_program build_program(cl_context ctx, cl_device_id device, const char* filename)
{
	cl_program program;
	cl_int err;
	char* buffer;
	char* program_log;
	size_t program_size, log_size;
	program_size = readfile(buffer, filename);
	program = clCreateProgramWithSource(ctx, 1, (const char**)&buffer, &program_size, &err);
	if (err < 0) {
		perror("Cannot create program\n");
		exit(1);
	}
	free(buffer);

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err < 0) {
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
			0, NULL, &log_size);
		program_log = (char*)malloc(log_size + 1);
		program_log[log_size] = '\0';
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
			log_size + 1, program_log, NULL);
		printf("%s\n", program_log);
		free(program_log);
		exit(1);
	}
	return program;
}

