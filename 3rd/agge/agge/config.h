#pragma once

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
	#define AGGE_INLINE __forceinline
#elif defined(__GNUC__) && (defined(__x86_64) || defined(__i386))
	#define AGGE_INLINE __attribute__((always_inline)) inline
#else
	#define AGGE_INLINE inline
#endif

#if defined(_M_IX86) || defined(__i386) || defined(_M_X64) || defined(__x86_64__)
	#define AGGE_ARCH_INTEL
#elif defined(_M_ARM)
	#define AGGE_ARCH_ARM _M_ARM
#elif defined(__arm__)
	#if defined(__ARM_ARCH_7__)
		#define AGGE_ARCH_ARM 7
	#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6T2__)
		#define AGGE_ARCH_ARM 6
	#elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__)
		#define AGGE_ARCH_ARM 5
	#else
		#define AGGE_ARCH_ARM 1
	#endif
#else
	#define AGGE_ARCH_GENERIC
#endif

#if defined(__ANDROID__)
	#define AGGE_PLATFORM_LINUX
	#define AGGE_PLATFORM_ANDROID
#elif defined(__linux__)
	#define AGGE_PLATFORM_LINUX
#elif defined(__APPLE__)
	#define AGGE_PLATFORM_APPLE
#elif defined(_WIN32)
	#define AGGE_PLATFORM_WINDOWS
#else
	#define AGGE_PLATFORM_GENERIC
#endif
