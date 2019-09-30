#include "../Include/DegraCore.h"

BOOL __stdcall Degra_Initialize ()
{
	/*dseed::add_bitmap_decoder (dseed::create_windows_imaging_codec_bitmap_decoder);
	dseed::add_bitmap_decoder (dseed::create_webp_bitmap_decoder);
	dseed::add_bitmap_decoder (dseed::create_jpeg_bitmap_decoder);
	dseed::add_bitmap_decoder (dseed::create_jpeg2000_bitmap_decoder);
	dseed::add_bitmap_decoder (dseed::create_png_bitmap_decoder);
	dseed::add_bitmap_decoder (dseed::create_gif_bitmap_decoder);
	dseed::add_bitmap_decoder (dseed::create_tga_bitmap_decoder);
	dseed::add_bitmap_decoder (dseed::create_tiff_bitmap_decoder);
	dseed::add_bitmap_decoder (dseed::create_dib_bitmap_decoder);*/

	return TRUE;
}

BOOL __stdcall Degra_Uninitialize ()
{
	return TRUE;
}

class __DegraStream : public dseed::stream
{
public:
	__DegraStream (const DegraStream_Initializer* initializer)
		: _refCount (1), _initializer (*initializer)
	{

	}

public:
	virtual int32_t retain () override { return ++_refCount; }
	virtual int32_t release () override
	{
		auto ret = --_refCount;
		if (ret == 0)
			delete this;
		return ret;
	}

public:
	virtual size_t read (void* buffer, size_t length) override
	{
		if (readable ())
			return _initializer.read (_initializer.user_data, buffer, length);
		return 0;
	}

	virtual size_t write (const void* data, size_t length) override
	{
		if (writable ())
			return _initializer.write (_initializer.user_data, data, length);
		return 0;
	}

	virtual bool seek (dseed::seekorigin_t origin, size_t offset) override
	{
		if (seekable ())
			return _initializer.seek (_initializer.user_data, origin, offset);
		return false;
	}

	virtual void flush () override
	{
		if (_initializer.flush != nullptr)
			_initializer.flush (_initializer.user_data);
	}

	virtual dseed::error_t set_length (size_t length) override { return dseed::error_not_impl; }

	virtual size_t position () override
	{
		return _initializer.position (_initializer.user_data);
	}

	virtual size_t length () override
	{
		return _initializer.length (_initializer.user_data);
	}

	virtual bool readable () override { return _initializer.read != nullptr; }
	virtual bool writable () override { return _initializer.write != nullptr; }
	virtual bool seekable () override { return _initializer.seek != nullptr; }

private:
	std::atomic<int32_t> _refCount;
	DegraStream_Initializer _initializer;
};

DegraStream __stdcall Degra_CreateStream (const DegraStream_Initializer* initializer)
{
	if (initializer->position == nullptr || initializer->length == nullptr)
		return nullptr;
	return new __DegraStream (initializer);
}
void __stdcall Degra_DestroyStream (DegraStream stream)
{
	stream->release ();
}

DegraImage __stdcall Degra_LoadImageFromStream (DegraStream stream)
{
	dseed::auto_object<dseed::bitmap_decoder> decoder;
	if (dseed::failed (dseed::detect_bitmap_decoder (stream, &decoder)))
		return nullptr;

	dseed::auto_object<dseed::bitmap> bitmap;
	if (dseed::failed (decoder->decode_frame (0, &bitmap, nullptr)))
		return nullptr;

	return bitmap.detach ();
}
void __stdcall Degra_DestroyImage (DegraImage image)
{
	image->release ();
}

void __stdcall Degra_GetImageSize (DegraImage image, UINT* width, UINT* height)
{
	auto size = image->size ();
	*width = size.width;
	*height = size.height;
}

DegraImage __stdcall Degra_ImagePixelFormatToPalette8Bit (DegraImage image)
{
	dseed::auto_object<dseed::bitmap> bitmap;
	if (dseed::failed (dseed::reformat_bitmap (image, dseed::pixelformat_bgra8888_indexed8, &bitmap)))
		return nullptr;
	return bitmap.detach ();
}
DegraImage __stdcall Degra_ImagePixelFormatToGrayscale (DegraImage image)
{
	dseed::auto_object<dseed::bitmap> bitmap;
	if (dseed::failed (dseed::reformat_bitmap (image, dseed::pixelformat_grayscale8, &bitmap)))
		return nullptr;
	return bitmap.detach ();
}
DegraImage __stdcall Degra_ImageResize (DegraImage image, DegraImage_ResizeFilter filter, int height)
{
	dseed::resize_t resize_method;
	switch (filter)
	{
	case DegraImage_ResizeFilter_Nearest: resize_method = dseed::resize_nearest; break;
	case DegraImage_ResizeFilter_Linear: resize_method = dseed::resize_bilinear; break;
	case DegraImage_ResizeFilter_Bicubic: resize_method = dseed::resize_bicubic; break;
	case DegraImage_ResizeFilter_Ranczos: resize_method = dseed::resize_lanczos; break;
	default: return nullptr;
	}

	auto size = image->size ();

	dseed::auto_object<dseed::bitmap> bitmap;
	if (dseed::failed (dseed::resize_bitmap (image, resize_method, dseed::size3i ((int)(size.width * (height / (float)size.height)), height, 1), &bitmap)))
		return nullptr;

	return bitmap.detach ();
}
DegraImage __stdcall Degra_ImageHistogramEqualization (DegraImage image)
{
	dseed::auto_object<dseed::bitmap> temp;
	if (dseed::failed (dseed::reformat_bitmap (image, dseed::pixelformat_hsva8888, &temp)))
		return nullptr;

	dseed::auto_object<dseed::bitmap> temp2;
	if (dseed::failed (dseed::bitmap_auto_histogram_equalization (temp, dseed::histogram_color_third, 0, &temp2)))
		return nullptr;

	dseed::auto_object<dseed::bitmap> temp3;
	if (dseed::failed (dseed::reformat_bitmap (temp2, dseed::pixelformat_rgba8888, &temp3)))
		return nullptr;

	return temp3.detach ();
}
BOOL __stdcall Degra_DetectTransparent (DegraImage image)
{
	bool transparent;
	dseed::bitmap_detect_transparent (image, &transparent);
	return transparent;
}

BOOL __stdcall Degra_SaveImageToStreamJPEG (DegraImage image, const JPEGOptions* options, DegraStream stream)
{
	dseed::jpeg_encoder_options encoderOptions;
	encoderOptions.quality = options->quality;

	dseed::auto_object<dseed::bitmap_encoder> encoder;
	if (dseed::failed (dseed::create_jpeg_bitmap_encoder (stream, &encoderOptions, &encoder)))
		return FALSE;

	if (dseed::failed (encoder->encode_frame (image, 0)))
		return FALSE;
	if (dseed::failed (encoder->commit ()))
		return FALSE;

	return TRUE;
}
BOOL __stdcall Degra_SaveImageToStreamWebP (DegraImage image, const WebPOptions* options, DegraStream stream)
{
	dseed::webp_encoder_options encoderOptions;
	encoderOptions.quality = options->quality;
	encoderOptions.lossless = options->lossless;

	dseed::auto_object<dseed::bitmap_encoder> encoder;
	if (dseed::failed (dseed::create_webp_bitmap_encoder (stream, &encoderOptions, &encoder)))
		return FALSE;

	if (dseed::failed (encoder->encode_frame (image, 0)))
		return FALSE;
	if (dseed::failed (encoder->commit ()))
		return FALSE;

	return TRUE;
}
BOOL __stdcall Degra_SaveImageToStreamPNG (DegraImage image, const PNGOptions* options, DegraStream stream)
{
	dseed::png_encoder_options encoderOptions;
	encoderOptions.use_zopfli_optimization = options->zopfli;

	dseed::auto_object<dseed::bitmap_encoder> encoder;
	if (dseed::failed (dseed::create_png_bitmap_encoder (stream, &encoderOptions, &encoder)))
		return FALSE;

	if (dseed::failed (encoder->encode_frame (image, 0)))
		return FALSE;
	if (dseed::failed (encoder->commit ()))
		return FALSE;

	return TRUE;
}