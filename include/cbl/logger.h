#pragma once

#ifndef SPDLOG_NO_NAME
#define SPDLOG_NO_NAME
#endif
#ifndef SPDLOG_ACTIVE_LEVEL 
#define SPDLOG_ACTIVE_LEVEL 0
#else
#undef SPDLOG_ACTIVE_LEVEL 
#define SPDLOG_ACTIVE_LEVEL 0
#endif
#include <cbl/third_party/spdlog/spdlog.h>

namespace cbl{
	//typedef typename spdlog::from_file from_file;
	//typedef typename spdlog::get get;
}
