#ifndef __DegraCore_h__
#define __DegraCore_h__

#include <dseed.h>

extern "C"
{
	typedef dseed::stream* DegraStream;
	typedef dseed::bitmap* DegraImage;

	typedef size_t (*DegraStream_Read)(void* user_data, void* buffer, size_t length);
	typedef size_t (*DegraStream_Write)(void* user_data, const void* buffer, size_t length);
	typedef bool (*DegraStream_Seek)(void* user_data, dseed::seekorigin_t origin, size_t offset);
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

	enum DegraImage_ResizeFilter
	{
		DegraImage_ResizeFilter_Nearest,
		DegraImage_ResizeFilter_Linear,
		DegraImage_ResizeFilter_Bicubic,
		DegraImage_ResizeFilter_Ranczos,
	};

	__declspec(dllexport) BOOL __stdcall Degra_Initialize ();
	__declspec(dllexport) BOOL __stdcall Degra_Uninitialize ();

	__declspec(dllexport) DegraStream __stdcall Degra_CreateStream (const DegraStream_Initializer* initializer);
	__declspec(dllexport) void __stdcall Degra_DestroyStream (DegraStream stream);

	__declspec(dllexport) DegraImage __stdcall Degra_LoadImageFromStream (DegraStream stream);
	__declspec(dllexport) void __stdcall Degra_DestroyImage (DegraImage image);

	__declspec(dllexport) void __stdcall Degra_GetImageSize (DegraImage image, UINT* width, UINT* height);

	__declspec(dllexport) DegraImage __stdcall Degra_ImagePixelFormatToPalette8Bit (DegraImage image);
	__declspec(dllexport) DegraImage __stdcall Degra_ImageResize (DegraImage image, DegraImage_ResizeFilter filter, int height);
	__declspec(dllexport) DegraImage __stdcall Degra_ImageHistogramEqualization (DegraImage image);

	struct JPEGOptions
	{
		UINT quality;
	};
	__declspec(dllexport) BOOL __stdcall Degra_SaveImageToStreamJPEG (DegraImage image, const JPEGOptions* options, DegraStream stream);
	struct WebPOptions
	{
		UINT quality;
		BOOL lossless;
	};
	__declspec(dllexport) BOOL __stdcall Degra_SaveImageToStreamWebP (DegraImage image, const WebPOptions* options, DegraStream stream);
	struct PNGOptions
	{
		BOOL zopfli;
	};
	__declspec(dllexport) BOOL __stdcall Degra_SaveImageToStreamPNG (DegraImage image, const PNGOptions* options, DegraStream stream);
}

#endif