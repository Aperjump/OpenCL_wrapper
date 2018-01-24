#include <cstdio>
#include <cstdlib>
#include <CL/cl.h>
#include "util.h"
const char* FILE_NAME =  "compute.cl";
int main()
{
	cl_device_id device = new_device();
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_int i, j, err;

	float matrix_1[80], matrix_2[80], matrix_3[80];
	const size_t buffer_origin[3] = { 5 * sizeof(float), 3, 0 };
	const size_t host_origin[3] = { 1 * sizeof(float), 1, 0 };
	const size_t region[3] = { 4 * sizeof(float), 4, 1 };
	cl_mem matrix_buffer_1, matrix_buffer_2, matrix_buffer_3;

	for (i = 0; i < 80; i++)
	{
		matrix_1[i] = i * 1.0f;
		matrix_2[i] = 3.0;
		matrix_3[i] = 0;
	}

	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if (err < 0)
	{
		perror("Couldn't create a context\n");
		exit(1);
	}
	program = build_program(context, device, FILE_NAME);
	kernel = clCreateKernel(program, "add", &err);
	if (err < 0) {
		perror("Couldn't create a kernel\n");
		exit(1);
	}
	
	matrix_buffer_1 = clCreateBuffer(context, CL_MEM_READ_WRITE |
		CL_MEM_COPY_HOST_PTR, sizeof(matrix_1), matrix_1, &err);
	if (err < 0) {
		perror("Couldn't create a buffer\n");
		exit(1);
	}
	matrix_buffer_2 = clCreateBuffer(context, CL_MEM_READ_WRITE |
		CL_MEM_COPY_HOST_PTR, sizeof(matrix_2), matrix_2, &err);
	if (err < 0) {
		perror("Couldn't create a buffer\n");
		exit(1);
	}
	matrix_buffer_3 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(matrix_3), matrix_3, &err);
	if (err < 0) {
		perror("Couldn't create a buffer\n");
		exit(1);
	}
	int row = 8;
	int col = 10;
	err = clSetKernelArg(kernel, 0, sizeof(int), &row);
	err = clSetKernelArg(kernel, 1, sizeof(int), &col);
	err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &matrix_buffer_1);
	err = clSetKernelArg(kernel, 3, sizeof(cl_mem), &matrix_buffer_2);
	err = clSetKernelArg(kernel, 4, sizeof(cl_mem), &matrix_buffer_3);

	queue = clCreateCommandQueue(context, device, 0, &err);
	if (err < 0) {
		perror("Couldn't create a command queue\n");
		exit(1);
	}
	err = clEnqueueTask(queue, kernel, 0, NULL, NULL);
	if (err < 0) {
		perror("Couldn't enque task\n");
		exit(1);
	}
	err = clEnqueueReadBuffer(queue, matrix_buffer_3, CL_TRUE, 0,
		sizeof(matrix_3), &matrix_3, 0, NULL, NULL);
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 10; j++) {
			printf("%6.1f ", matrix_3[j + i * 10]);
		}
		printf("\n");
	}
	clReleaseMemObject(matrix_buffer_1);
	clReleaseMemObject(matrix_buffer_2);
	clReleaseMemObject(matrix_buffer_3);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	return 0;
}