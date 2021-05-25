#ifndef __DegraCore_h__
#define __DegraCore_h__

#include <dseed.h>

extern "C"
{
	typedef dseed::io::stream* DegraStream;
	typedef dseed::bitmaps::bitmap* DegraImage;

	typedef size_t (*DegraStream_Read)(void* user_data, void* buffer, size_t length);
	typedef size_t (*DegraStream_Write)(void* user_data, const void* buffer, size_t length);
	typedef bool (*DegraStream_Seek)(void* user_data, dseed::io::seekorigin origin, size_t offset);
	typedef void (*DegraStream_Flush)(void* user_data);
	typedef size_t (*DegraStream_Position)(void* user_data);
	typedef size_t (*DegraStream_Length)(void* user_data);
	
	struct DegraStream_Initializer
	{
		void* user_data;
		DegraStream_Read read;
		DegraStream_Write write;
		DegraStream_Seek seek;
		DegraStream_Flush flush;
		DegraStream_Position position;
		DegraStream_Length length;
	};

	enum class DegraResizeFilter : unsigned int
	{
		Nearest,
		Linear,
		Bicubic,
		Lanczos,
		LanczosX5,
	};

	enum class DegraSaveFormat : unsigned int
	{
		SameFormat,
		Png,
		Jpeg,
		WebP,
	};

	struct DegraOptions
	{
		DegraSaveFormat save_format;
		UINT quality;
		UINT max_height;
		DegraResizeFilter resize_filter;
		BOOL use_lossless;
		BOOL use_8bit_palette;
		BOOL use_8bit_palette_but_no_use_over_256_color;
		BOOL use_grayscale;
		BOOL use_grayscale_but_no_use_to_grayscale_image;
		BOOL no_convert_to_png_when_detected_transparent_color;
	};

	enum class DegraResult
	{
		Failed,
		Passed,
		Succeeded,
	};

	__declspec(dllexport) BOOL __stdcall Degra_Initialize ();
	__declspec(dllexport) BOOL __stdcall Degra_Uninitialize ();

	__declspec(dllexport) DegraStream __stdcall Degra_CreateStream (const DegraStream_Initializer* initializer);
	__declspec(dllexport) void __stdcall Degra_DestroyStream (DegraStream stream);

	__declspec(dllexport) DegraResult __stdcall Degra_DoProcess(DegraStream inputStream, DegraStream outputStream, const DegraOptions* options, DegraSaveFormat* savedFormat);
}

#endif