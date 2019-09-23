#ifndef __DSEED_COMMON_H__
#define __DSEED_COMMON_H__

#include <atomic>
#include <string>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cmath>
#include <functional>
#include <random>
using namespace std::string_literals;

#define COMPILER_MSVC										defined ( _MSC_VER )
#define COMPILER_GCC										defined ( __GNUC__ ) && !defined ( __clang__ )
#define COMPILER_CLANG										defined ( __clang__ )

#define PLATFORM_ANDROID									( defined ( __ANDROID__ ) )
#define PLATFORM_FUCHSIA									( defined ( __Fuchsia__ ) )
#define PLATFORM_GOOGLE										( PLATFORM_ANDROID || PLATFORM_FUCHSIA )
#define PLATFORM_UNIX										( defined ( __unix__ ) || defined ( __linux__ ) ) && !defined ( __ANDROID__ )
#define PLATFORM_WEBASSEMBLY								( defined ( __EMSCRIPTEN__ ) || defined ( __asmjs__ ) )
#if defined ( __APPLE__ )
#	include <TargetConditionals.h>
#	define PLATFORM_IOS										( TARGET_OS_IOS || TARGET_OS_SIMULATOR )
#	define PLATFORM_MACOS									( TARGET_OS_MAC && !PLATFORM_IOS )
#else
#	define PLATFORM_IOS										0
#	define PLATFORM_MACOS									0
#endif
#define PLATFORM_APPLE										( PLATFORM_IOS || PLATFORM_MACOS )
#if ( defined ( _WINDOWS ) || defined ( _WIN32 ) || defined ( _WIN64 ) ) \
	&& !( PLATFORM_APPLE || PLATFORM_GOOGLE || PLATFORM_UNIX || PLATFORM_WEBASSEMBLY )
#	include <Windows.h>
#	define PLATFORM_UWP										( defined ( WINAPI_FAMILY ) && ( WINAPI_FAMILY == WINAPI_FAMILY_APP ) )
#	define PLATFORM_WINDOWS									( !defined ( WINAPI_FAMILY ) || ( WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP ) )
#	if PLATFORM_UWP
#		include <wrl.h>
#	endif
#else
#	define PLATFORM_WINDOWS									0
#	define PLATFORM_UWP										0
#endif
#define PLATFORM_MICROSOFT									( PLATFORM_WINDOWS || PLATFORM_UWP )

#define PLATFORM_DESKTOP									( PLATFORM_WINDOWS || PLATFORM_MACOS || PLATFORM_UNIX )
#define PLATFORM_HANDHELD									( PLATFORM_IOS || PLATFORM_ANDROID )
#define PLATFORM_UNIVERSAL									( PLATFORM_UWP || PLATFORM_FUCHSIA )

#if PLATFORM_MICROSOFT
constexpr char PATH_SEPARATOR = '\\';
#else
constexpr char PATH_SEPARATOR = '/';
#endif

#define ARCH_IA32											( defined ( _M_IX86 ) || defined ( __i386__ ) )
#define ARCH_AMD64											( defined ( _M_AMD64 ) || defined ( __amd64__ ) )
#define ARCH_X86SET											( ARCH_IA32 || ARCH_AMD64 )
#define ARCH_ARM											( defined ( _M_ARM ) || defined ( __arm__ ) )
#define ARCH_ARM64											( defined ( _M_ARM64 ) || defined ( __aarch64__ ) )
#define ARCH_ARMSET											( ARCH_ARM || ARCH_ARM64 )
#define ARCH_RISCV32										( ( defined ( __riscv ) || defined ( __riscv__ ) ) && ( __riscv_xlen == 32 ) )
#define ARCH_RISCV64										( ( defined ( __riscv ) || defined ( __riscv__ ) ) && ( __riscv_xlen == 64 ) )
#define ARCH_RISCVSET										( ARCH_RISCV32 || ARCH_RISCV64 )

#if ARCH_X86SET
#	include <immintrin.h>
#endif

#if COMPILER_MSVC && defined ( DSEED_WINDOWS_DLL_EXPORT )
#   define DSEEDEXP											__declspec ( dllexport )
#elif COMPILER_MSVC && !defined ( DSEED_WINDOWS_DLL_EXPORT )
#   define DSEEDEXP											__declspec ( dllimport )
#else
#   define DSEEDEXP
#endif

#ifndef PURE
#define PURE												= 0
#endif

#if COMPILER_MSVC
#	define ALIGN(x)											__declspec(align(x))
#else
#	define ALIGN(x)											__attribute__ ((aligned (x)))
#endif

namespace dseed
{
	using error_t = int32_t;
	constexpr bool succeeded (error_t err) { return err >= 0; }
	constexpr bool failed (error_t err) { return err < 0; }

	constexpr error_t error_good = 0;

	constexpr error_t error_fail = 0x80000000;
	constexpr error_t error_version_mismatch = 0x80000001;

	constexpr error_t error_invalid_args = 0x80010001;
	constexpr error_t error_invalid_op = 0x80010002;
	constexpr error_t error_out_of_range = 0x80010003;
	constexpr error_t error_out_of_memory = 0x80010004;
	constexpr error_t error_not_contained = 0x80010005;
	constexpr error_t error_type_incorrect = 0x80010006;

	constexpr error_t error_io = 0x80020001;
	constexpr error_t error_file_not_found = 0x80020002;
	constexpr error_t error_end_of_file = 0x80020003;
	constexpr error_t error_hash_not_correct = 0x80020004;
	constexpr error_t error_access_denied = 0x80020005;
	constexpr error_t error_not_support_file_format = 0x80020006;

	constexpr error_t error_not_impl = 0x80030001;
	constexpr error_t error_not_support = 0x80030002;
	constexpr error_t error_platform_not_support = 0x80030003;
	constexpr error_t error_feature_not_support = 0x80030004;

	constexpr error_t error_device_not_conn = 0x80040001;
	constexpr error_t error_device_disconn = 0x80040002;
	constexpr error_t error_device_reset = 0x80040003;

	constexpr error_t error_network_not_conn = 0x80050001;
	constexpr error_t error_network_disconn = 0x80060002;
	constexpr error_t error_network_sync_fail = 0x80060003;
}

namespace dseed
{
#	if COMPILER_MSVC
#		pragma pack (push, 1)
#	else
#		pragma pack (1)
#	endif
#	if ARCH_X86SET
	struct DSEEDEXP x86_instruction_info
	{
	public:
		char cpu_vender[64];								//< CPU Vender(Intel, AMD, ...)
		char cpu_brand[64];									//< CPU Brand(i5-4790K, Ryzen 5 2600X, ...)

		bool sse : 1,										//< SSE
			sse2 : 1,										//< SSE 2
			sse3 : 1,										//< SSE 3
			ssse3 : 1,										//< SSSE 3
			sse4_1 : 1,										//< SSE 4.1
			sse4_2 : 1,										//< SSE 4.2
			sse4a : 1;										//< SSE 4a
		bool avx : 1,										//< AVX (Fp 256-bit operators)
			avx2 : 1,										//< AVX 2 (Integer 256-bit operators)
			fma3 : 1;										//< FMA3 (Fused Multiply-Add)

		bool rdrand : 1,									//< RDRAND(Hardware Random Number Generator)
			aes : 1,										//< AES Encrypter/Decrypter
			sha : 1;										//< SHA Hash

		bool f16c : 1;										//< F16C(Half-precision <-> Single-precision Conversion)

		bool tsx : 1,										//< Transactional Synchronization Extensions(for Intel)
			asf : 1;										//< Advanced Synchronization Facility(for AMD)

		static const x86_instruction_info& instance ();

	private:
		x86_instruction_info ();
	};
#	endif

#	if ARCH_ARMSET
	struct DSEEDEXP arm_instruction_info
	{
	public:

		bool neon : 1,										//< ARM Neon SIMD operators
			crc32 : 1;										//< CRC32

		static const arm_instruction_info& instance ();

	private:
		arm_instruction_info ();
	};
#	endif
#	if COMPILER_MSVC
#		pragma pack (pop)
#	else
#		pragma pack ()
#	endif

	inline uint16_t hwrand16_def () noexcept
	{
		static std::random_device rd;
		static std::mt19937 rnd (rd ());
		static std::uniform_int_distribution<uint16_t> range;
		return range (rnd);
	}
	inline uint32_t hwrand32_def () noexcept
	{
		static std::random_device rd;
		static std::mt19937 rnd (rd ());
		static std::uniform_int_distribution<uint32_t> range;
		return range (rnd);
	}
	inline uint64_t hwrand64_def () noexcept
	{
		static std::random_device rd;
		static std::mt19937_64 rnd (rd ());
		static std::uniform_int_distribution<uint64_t> range;
		return range (rnd);
	}

#if ARCH_X86SET
	inline uint16_t hwrand16_x86 () noexcept
	{
		uint16_t ret;
		_rdrand16_step (&ret);
		return ret;
	}
	inline uint32_t hwrand32_x86 () noexcept
	{
		uint32_t ret;
		_rdrand32_step (&ret);
		return ret;
	}
	inline uint64_t hwrand64_x86 () noexcept
	{
		uint64_t ret;
		_rdrand64_step (&ret);
		return ret;
	}

	inline uint16_t hwrand16 () noexcept { return hwrand16_x86 (); }
	inline uint32_t hwrand32 () noexcept { return hwrand32_x86 (); }
	inline uint64_t hwrand64 () noexcept { return hwrand64_x86 (); }
#else
	inline uint16_t hwrand16 () noexcept { return hwrand16_def (); }
	inline uint32_t hwrand32 () noexcept { return hwrand32_def (); }
	inline uint64_t hwrand64 () noexcept { return hwrand64_def (); }
#endif

	DSEEDEXP uint32_t crc32 (uint32_t crc32, const uint8_t* bytes, size_t bytesCount) noexcept;
}

#if PLATFORM_WINDOWS
#	define ENTRYPOINT_ATTRIBUTE								
#	define ENTRYPOINT_RETURNTYPE							int
#	define ENTRYPOINT_CALLTYPE								__stdcall
#	define ENTRYPOINT_NAME									WinMain
#	define ENTRYPOINT_ARGUMENTS								HINSTANCE, HINSTANCE, LPSTR, int
#elif PLATFORM_UWP
#	define ENTRYPOINT_ATTRIBUTE								[Platform::MTAThread]
#	define ENTRYPOINT_RETURNTYPE							int
#	define ENTRYPOINT_CALLTYPE								__cdecl
#	define ENTRYPOINT_NAME									main
#	define ENTRYPOINT_ARGUMENTS								Platform::Array<Platform::String^>^
#elif PLATFORM_ANDROID
#	define ENTRYPOINT_ATTRIBUTE								
#	define ENTRYPOINT_RETURNTYPE							void
#	define ENTRYPOINT_CALLTYPE								
#	define ENTRYPOINT_NAME									android_main
#	define ENTRYPOINT_ARGUMENTS								struct android_app* app
#else
#	define ENTRYPOINT_ATTRIBUTE								
#	define ENTRYPOINT_RETURNTYPE							int
#	define ENTRYPOINT_CALLTYPE								
#	define ENTRYPOINT_NAME									main
#	define ENTRYPOINT_ARGUMENTS								int, char*[]
#endif

#endif