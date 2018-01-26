#include "openCL_wrapper.h"

namespace OpenCL_wrapper {
	cl_platform_id choose_platform(const std::string& pltfrm_name) {
		cl_uint platformnum = 0;
		cl_int err = clGetPlatformIDs(0, NULL, &platformnum);
		if (err < 0) {
			perror("query platform error\n");
			exit(1);
		}
		cl_platform_id* platform = (cl_platform_id*)malloc(sizeof(cl_platform_id) * platformnum);
		cl_int err = clGetPlatformIDs(platformnum, platform, NULL);
		if (err < 0) {
			perror("get platform vector error\n");
			exit(1);
		}
		// Query info for each devices and try to match pltfrm_name
		for (cl_int i = 0; i < platformnum; i++)
		{
			size_t plt_name_len = 0;
			err = clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, 0, 0, &plt_name_len);
			if (err < 0) {
				perror("cannot find platform\n");
				exit(1);
			}
			std::vector<char> plt_name(plt_name_len);
			err = clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, plt_name_len,
				&plt_name[0], NULL);
			if (err < 0) {
				perror("cannot find platform\n");
				exit(1);
			}
			if (std::string(&plt_name[0]).find(pltfrm_name) != std::string::npos)
			{
				cl_platform_id ret = platform[i];
				free(platform);
				return ret;
			}
		}
		// may have truble
		return NULL;
	}

	bool CL_Base::init(const std::string& device_type, cl_command_queue_properties queue_prop = 0,
		const cl_context_properties* additional_context_props = 0) {

	}
};
