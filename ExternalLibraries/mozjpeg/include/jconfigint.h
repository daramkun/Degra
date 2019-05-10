#define VERSION "3.3.1"
#define BUILD "20190510"
#define PACKAGE_NAME "mozjpeg"

#ifndef INLINE
#if defined(__GNUC__)
#define INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define INLINE __forceinline
#else
#define INLINE
#endif
#endif
