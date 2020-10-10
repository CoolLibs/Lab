#pragma once

#ifdef _MSC_VER
	#define debug_break __debugbreak
#else
	#define debug_break __builtin_trap
#endif
