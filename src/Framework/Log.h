#pragma once

#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>

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
		debug_break();
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
		}
	}
}