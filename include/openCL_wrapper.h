#pragma once
#include <CL/cl.h>
#include <cstdlib>
#include <string>
#include <vector>
namespace OpenCL_wrapper {
	// choose platform given platform name 
	cl_platform_id choose_platform(const std::string& pltfrm_name);
	// offset is device's position in a vector<cl_device_id>
	// this function will call choose_mult_device and select one with offset
	cl_device_id choose_device(cl_platform_id platform, std::string& devs_name, int offset);
	std::vector<cl_device_id> choose_mult_device(cl_platform_id, std::string& device_type);
	// contains all required structure for computation
	class CL_Base {
	public:
		CL_Base(const std::string& pltform_name) { _platform = OpenCL_wrapper::choose_platform(pltform_name); }
		CL_Base(cl_platform_id platform) : _platform(platform) { }
		~CL_Base();
		bool init(const std::string& device_type, cl_command_queue_properties queue_prop = 0, 
			const cl_context_properties* additional_context_props = 0);
		cl_device_id switch_device(int offset) { current_device = _devices[offset]; return current_device; }
		cl_platform_id get_platform() { return _platform;  }
		cl_device_id get_device() { return current_device; }
		cl_context get_context() { return _context; }
		cl_command_queue get_cmdque() { return _cmdque; }
	private:
		cl_platform_id _platform;
		cl_device_id current_device;
		std::vector<cl_device_id> _devices;
		cl_context _context;
		cl_command_queue _cmdque;
	};
};