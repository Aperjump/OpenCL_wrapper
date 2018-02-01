#pragma once
#include <CL/cl.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
namespace ww_matrix {
	// choose platform given platform name 
	cl_platform_id choose_platform(const std::string& pltfrm_name);
	// offset is device's position in a vector<cl_device_id>
	// this function will call choose_mult_device and select one with offset
	cl_device_id choose_device(cl_platform_id platform, const std::string& devs_name, size_t offset);
	std::vector<cl_device_id> choose_mult_device(cl_platform_id, const std::string& device_type);
	// contains all required structure for computation
	class CL_Base {
	public:
		CL_Base(const std::string& pltform_name) { _platform = ww_matrix::choose_platform(pltform_name); }
		CL_Base(cl_platform_id platform) : _platform(platform) { }
		CL_Base(CL_Base& other) : _platform(other._platform),
			current_device(other.current_device), _devices(other._devices),
			_context(other._context), _cmdque(other._cmdque), _program(other._program) { }
		CL_Base& operator= (const CL_Base& other);
		~CL_Base() { }
		bool init(const std::string& device_type, cl_command_queue_properties queue_prop, 
			const cl_context_properties* additional_context_props);
		cl_device_id switch_device(int offset);
		bool create_program(const std::string& file);
		//bool create_kernels();

		cl_platform_id _platform;
		cl_device_id current_device;
		std::vector<cl_device_id> _devices;
		cl_context _context;
		cl_command_queue _cmdque;
		cl_program _program;
		//cl_kernel* _kernels;
		//cl_uint kernel_num;
	};
};