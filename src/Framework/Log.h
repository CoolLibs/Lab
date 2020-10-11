#pragma once

#include <spdlog/spdlog.h>

namespace Log {
	inline void Initialize() {
		spdlog::set_pattern("%^[%e] %n: %v%$");
	}
	// 
	template<typename ...Args> inline void Info(Args&& ...args) {
#ifndef NDEBUG
		spdlog::info(std::forward<Args>(args)...);
#endif
	}
	template<typename ...Args> inline void Warn(Args&& ...args) {
#ifndef NDEBUG
		spdlog::warn(std::forward<Args>(args)...);
#endif
	}
	template<typename ...Args> inline void Error(Args&& ...args) {
#ifndef NDEBUG
		spdlog::error(std::forward<Args>(args)...);
		assert(false);
#endif
	}
	template<typename ...Args> inline void ErrorWithoutBreakpoint(Args&& ...args) {
#ifndef NDEBUG
		spdlog::error(std::forward<Args>(args)...);
#endif
	}
	//
	namespace Release {
		template<typename ...Args> inline void Info(Args&& ...args) {
			spdlog::info(std::forward<Args>(args)...);
		}
		template<typename ...Args> inline void Warn(Args&& ...args) {
			spdlog::warn(std::forward<Args>(args)...);
		}
		template<typename ...Args> inline void Error(Args&& ...args) {
			spdlog::error(std::forward<Args>(args)...);
			assert(false);
		}
	}
}