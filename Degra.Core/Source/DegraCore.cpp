#include "../Include/DegraCore.h"

const int BUFFER_SIZE = 4096;

dseed::bitmaps::decoder_creator_func g_imageDecoders[] = {
	nullptr,

	dseed::bitmaps::create_png_bitmap_decoder,
	dseed::bitmaps::create_jpeg_bitmap_decoder,
	dseed::bitmaps::create_webp_bitmap_decoder,

	dseed::bitmaps::create_dib_bitmap_decoder,
	dseed::bitmaps::create_jpeg2000_bitmap_decoder,
	dseed::bitmaps::create_tga_bitmap_decoder,
	dseed::bitmaps::create_tiff_bitmap_decoder,
	dseed::bitmaps::create_cur_bitmap_decoder,
	dseed::bitmaps::create_ico_bitmap_decoder,
	dseed::bitmaps::create_gif_bitmap_decoder,
};

dseed::bitmaps::encoder_creator_func g_imageEncoders[] = {
	nullptr,

	dseed::bitmaps::create_png_bitmap_encoder,
	dseed::bitmaps::create_jpeg_bitmap_encoder,
	dseed::bitmaps::create_webp_bitmap_encoder,
};

BOOL __stdcall Degra_Initialize()
{
	return TRUE;
}

BOOL __stdcall Degra_Uninitialize()
{
	return TRUE;
}

class __DegraStream final : public dseed::io::stream
{
public:
	__DegraStream(const DegraStream_Initializer* initializer)
		: _refCount(1), _initializer(*initializer)
	{

	}

public:
	virtual int32_t retain() override { return ++_refCount; }
	virtual int32_t release() override
	{
		auto ret = --_refCount;
		if (ret == 0)
			delete this;
		return ret;
	}

public:
	virtual size_t read(void* buffer, size_t length) noexcept override
	{
		if (readable())
			return _initializer.read(_initializer.user_data, buffer, length);
		return 0;
	}

	virtual size_t write(const void* data, size_t length) noexcept override
	{
		if (writable())
			return _initializer.write(_initializer.user_data, data, length);
		return 0;
	}

	virtual bool seek(dseed::io::seekorigin origin, size_t offset) noexcept override
	{
		if (seekable())
			return _initializer.seek(_initializer.user_data, origin, offset);
		return false;
	}

	virtual void flush() noexcept override
	{
		if (_initializer.flush != nullptr)
			_initializer.flush(_initializer.user_data);
	}

	virtual dseed::error_t set_length(size_t length) noexcept override { return dseed::error_not_impl; }

	virtual size_t position() noexcept override
	{
		return _initializer.position(_initializer.user_data);
	}

	virtual size_t length() noexcept override
	{
		return _initializer.length(_initializer.user_data);
	}

	virtual bool readable() noexcept override { return _initializer.read != nullptr; }
	virtual bool writable() noexcept override { return _initializer.write != nullptr; }
	virtual bool seekable() noexcept override { return _initializer.seek != nullptr; }

private:
	std::atomic<int32_t> _refCount;
	DegraStream_Initializer _initializer;
};

DegraStream __stdcall Degra_CreateStream(const DegraStream_Initializer* initializer)
{
	if (initializer->position == nullptr || initializer->length == nullptr)
		return nullptr;
	return new __DegraStream(initializer);
}
void __stdcall Degra_DestroyStream(DegraStream stream)
{
	stream->release();
}

void __copy_stream(dseed::io::stream* inputStream, dseed::io::stream* outputStream)
{
	size_t total_read = 0;
	while (total_read != inputStream->length())
	{
		BYTE buffer[BUFFER_SIZE];
		const auto read = inputStream->read(buffer, sizeof(buffer));
		outputStream->write(buffer, read);
		total_read += read;
	}
}

dseed::bitmaps::bitmap* __image_resize(dseed::bitmaps::bitmap* image, DegraResizeFilter filter, int height)
{
	dseed::bitmaps::resize resize_method;
	switch (filter)
	{
	case DegraResizeFilter::Nearest: resize_method = dseed::bitmaps::resize::nearest; break;
	case DegraResizeFilter::Linear: resize_method = dseed::bitmaps::resize::bilinear; break;
	case DegraResizeFilter::Bicubic: resize_method = dseed::bitmaps::resize::bicubic; break;
	case DegraResizeFilter::Ranczos: resize_method = dseed::bitmaps::resize::lanczos; break;
	case DegraResizeFilter::RanczosX5: resize_method = dseed::bitmaps::resize::lanczos5; break;
	default: return nullptr;
	}

	auto size = image->size();

	dseed::autoref<dseed::bitmaps::bitmap> bitmap;
	if (dseed::failed(dseed::bitmaps::resize_bitmap(image, resize_method, dseed::size3i((int)(size.width * (height / (float)size.height)), height, 1), &bitmap)))
		return nullptr;

	return bitmap.detach();
}

dseed::bitmaps::bitmap* __image_pixel_format_to_palette_8bit(dseed::bitmaps::bitmap* image)
{
	dseed::autoref<dseed::bitmaps::bitmap> bitmap;
	if (dseed::failed(dseed::bitmaps::reformat_bitmap(image, dseed::color::pixelformat::bgra8_indexed8, &bitmap)))
		return nullptr;
	return bitmap.detach();
}

dseed::bitmaps::bitmap* __image_pixel_format_to_grayscale(dseed::bitmaps::bitmap* image)
{
	dseed::autoref<dseed::bitmaps::bitmap> bitmap;
	if (dseed::failed(dseed::bitmaps::reformat_bitmap(image, dseed::color::pixelformat::r8, &bitmap)))
		return nullptr;
	return bitmap.detach();
}

dseed::bitmaps::bitmap* __stdcall __image_histogram_equalization(dseed::bitmaps::bitmap* image)
{
	dseed::autoref<dseed::bitmaps::bitmap> temp;
	if (dseed::failed(dseed::bitmaps::reformat_bitmap(image, dseed::color::pixelformat::hsva8, &temp)))
		return nullptr;

	dseed::autoref<dseed::bitmaps::bitmap> temp2;
	if (dseed::failed(dseed::bitmaps::bitmap_auto_histogram_equalization(temp, dseed::bitmaps::histogram_color::third, 0, &temp2)))
		return nullptr;

	dseed::autoref<dseed::bitmaps::bitmap> temp3;
	if (dseed::failed(dseed::bitmaps::reformat_bitmap(temp2, dseed::color::pixelformat::rgba8, &temp3)))
		return nullptr;

	return temp3.detach();
}

DegraResult __stdcall Degra_DoProcess(DegraStream inputStream, DegraStream outputStream, const DegraOptions* options, DegraSaveFormat* savedFormat)
{
	if (inputStream == nullptr || outputStream == nullptr || options == nullptr || savedFormat == nullptr)
		return DegraResult::Failed;
	
	dseed::autoref<dseed::bitmaps::bitmap_array> bitmap_array;

	auto open_format = DegraSaveFormat::SameFormat;
	for (size_t i = 0; i < _countof(g_imageDecoders); ++i)
	{
		const auto decoder = g_imageDecoders[i];
		
		if (decoder == nullptr)
			continue;
		
		if (!inputStream->seek(dseed::io::seekorigin::begin, 0))
			return DegraResult::Failed;
		
		if (dseed::succeeded(decoder(inputStream, &bitmap_array)))
		{
			open_format = static_cast<DegraSaveFormat>(i);
			break;
		}

		if (bitmap_array)
		{
			bitmap_array.release();
		}
	}
	
	bool pass = bitmap_array == nullptr;
	DegraSaveFormat save_format = options->save_format;
	if (save_format == DegraSaveFormat::SameFormat)
	{
		if (open_format == DegraSaveFormat::Png)
			save_format = DegraSaveFormat::Png;
		else if (open_format == DegraSaveFormat::Jpeg)
			save_format = DegraSaveFormat::Jpeg;
		else if (open_format == DegraSaveFormat::WebP)
			save_format = DegraSaveFormat::WebP;
		else
			pass = true;
	}

	if (pass)
	{
		__copy_stream(inputStream, outputStream);
		return DegraResult::Passed;
	}

	std::vector<dseed::autoref<dseed::bitmaps::bitmap>> targets;
	for (size_t i = 0; i < bitmap_array->size(); ++i)
	{
		dseed::autoref<dseed::bitmaps::bitmap> bitmap;
		if (dseed::failed(bitmap_array->at(i, &bitmap)))
			continue;

		if (const auto bitmap_size = bitmap->size(); bitmap_size.height > options->max_height)
			bitmap = __image_resize(bitmap, options->resize_filter, options->max_height);

		dseed::bitmaps::bitmap_properties prop = {};
		if (options->use_8bit_palette_but_no_use_over_256_color ||
			options->use_grayscale_but_no_use_to_grayscale_image)
		{
			if (dseed::failed(dseed::bitmaps::determine_bitmap_properties(bitmap, &prop)))
				continue;
		}
		
		if (save_format == DegraSaveFormat::Png && options->use_8bit_palette)
		{
			if (!options->use_8bit_palette_but_no_use_over_256_color ||
				(
					options->use_8bit_palette_but_no_use_over_256_color &&
					prop.colours != dseed::bitmaps::colorcount::color_cannot_palettable
				)
			)
			{
				bitmap = __image_pixel_format_to_palette_8bit(bitmap);
			}
		}

		if (save_format == DegraSaveFormat::Png && options->no_convert_to_png_when_detected_transparent_color && prop.transparent)
		{
			__copy_stream(inputStream, outputStream);
			return DegraResult::Passed;
		}

		if (options->use_grayscale)
		{
			if (!options->use_grayscale_but_no_use_to_grayscale_image ||
				(
					options->use_grayscale_but_no_use_to_grayscale_image &&
					prop.grayscale
				)
			)
			{
				bitmap = __image_pixel_format_to_grayscale(bitmap);
			}
		}

		if (options->use_histogram_equailization)
		{
			bitmap = __image_histogram_equalization(bitmap);
		}
		
		targets.push_back(bitmap);
	}

	if (targets.empty())
		return DegraResult::Failed;

	dseed::autoref<dseed::bitmaps::bitmap_encoder> encoder;
	bool is_multi_frame_format = false;
	switch (save_format)
	{
	case DegraSaveFormat::Png:
		if (dseed::failed(dseed::bitmaps::create_png_bitmap_encoder(outputStream, nullptr, &encoder)))
			return DegraResult::Failed;
		break;

	case DegraSaveFormat::Jpeg:
		{
			dseed::bitmaps::jpeg_encoder_options encoder_options = {};
			encoder_options.quality = options->quality;
			if (dseed::failed(dseed::bitmaps::create_jpeg_bitmap_encoder(outputStream, &encoder_options, &encoder)))
				return DegraResult::Failed;
			}
		break;

	case DegraSaveFormat::WebP:
		{
			dseed::bitmaps::webp_encoder_options encoder_options = {};
			encoder_options.quality = options->quality;
			encoder_options.lossless = options->use_lossless;
			if (dseed::failed(dseed::bitmaps::create_webp_bitmap_encoder(outputStream, &encoder_options, &encoder)))
				return DegraResult::Failed;
			is_multi_frame_format = true;
		}
		break;
		
	default:
		return DegraResult::Failed;
	}

	if (encoder == nullptr)
		return DegraResult::Failed;

	for (auto& target : targets)
	{
		encoder->encode_frame(target);
		
		if (!is_multi_frame_format)
			break;
	}
	encoder->commit();

	outputStream->flush();
	*savedFormat = save_format;

	return DegraResult::Succeeded;
}