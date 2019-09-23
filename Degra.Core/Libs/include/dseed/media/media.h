#ifndef __DSEED_MEDIA_MEDIA_H__
#define __DSEED_MEDIA_MEDIA_H__

namespace dseed
{
	// Media Sample Types
	using sampletype_t = uint32_t;
	enum
	{
		sampletype_unknown = 0,
		
		sampletype_image = 0x00000001,
		sampletype_audio = 0x00000002,

		sampletype_decoded = 0x00000000,
		sampletype_encoded = 0x00010000,

		sampletype_out_of_range = 0xffffffff,
	};

	// Image Format
	struct DSEEDEXP imageformat
	{
		size2i size;
		pixelformat_t image_format;
		fraction frame_rate;

		imageformat () = default;
		imageformat (const size2i& size, pixelformat_t format, fraction framerate)
			: size (size), image_format (format), frame_rate (framerate)
		{ }
	};

	// Audio Pulse Format
	enum pulseformat_t : int32_t
	{
		// Unknown Format
		pulseformat_unknown,

		// Integer PCM Format
		pulseformat_pcm,
		// IEEE Float PCM Format
		pulseformat_ieee_float,
	};

	// Audio Format
	struct DSEEDEXP audioformat
	{
		// Audio Channels (1: Mono, 2: Stereo, 3: 2.1ch, 6: 5.1ch, 8: 7.1ch)
		uint8_t channels;
		// Bits per Sample (8, 16, 24, 32)
		uint8_t bits_per_sample;
		// Block Alignment (Audio Channels * Bits per Sample / 8)
		uint16_t block_align;
		// Sample-Rate (44100, 48000, ...)
		uint32_t samples_per_sec;
		// Bytes per Second (Block Alignment * Sample-Rate)
		uint32_t bytes_per_sec;
		// Audio Pulse Sample Format
		pulseformat_t pulse_format;

		audioformat () = default;
		audioformat (int channels, int bits_per_sample, int samples_per_sec, pulseformat_t format)
			: channels (channels), bits_per_sample (bits_per_sample), samples_per_sec (samples_per_sec)
			, pulse_format (format)
			, block_align (channels* (bits_per_sample / 8)), bytes_per_sec (block_align* samples_per_sec)
		{ }
	};

	// Get Audio Duration from Bytes
	DSEEDEXP timespan_t get_audio_duration (const audioformat& wf, size_t bytes);
	// Get Bytes Count from Duration
	DSEEDEXP size_t get_audio_bytes_count (const audioformat& wf, dseed::timespan_t duration);
	// Get Bytes Count from Duration
	DSEEDEXP size_t get_audio_bytes_count (uint32_t bytes_per_sec, dseed::timespan_t duration);

	// Media Sample
	class DSEEDEXP media_sample : public dseed::object
	{
	public:
		virtual sampletype_t sample_type () = 0;

	public:
		// Get Sample Position
		virtual timespan_t sample_time () = 0;
		// Get Sample Length
		virtual timespan_t sample_duration () = 0;

	public:
		// Copy Sample Data to buffer
		virtual dseed::error_t copy_sample (void* buffer) = 0;
		// Get Sample BUffer Size
		virtual size_t sample_size () = 0;
	};

	// Media Decoder
	class DSEEDEXP media_decoder : public dseed::object
	{
	public:
		// Get Total Duration
		virtual dseed::timespan_t duration () = 0;

	public:
		virtual sampletype_t sample_type (size_t streamIndex) = 0;

	public:
		virtual error_t sample_format (size_t streamIndex, void* buffer, size_t bufferSize) = 0;

	public:
		// Get Reading Position
		virtual timespan_t reading_position () = 0;
		// Set Reading Position
		virtual error_t set_reading_position (dseed::timespan_t pos) = 0;

	public:
		virtual error_t decode (size_t streamIndex, media_sample** sample) = 0;
	};

	using createmediadecoder_fn = error_t (*) (dseed::stream*, media_decoder**);
	DSEEDEXP error_t add_media_decoder (createmediadecoder_fn fn);
	DSEEDEXP error_t detect_media_decoder (dseed::stream* stream, dseed::media_decoder** decoder);

	// Media Encoder
	class DSEEDEXP media_encoder : public dseed::object
	{
	public:
		virtual error_t set_sample_format (size_t streamIndex, sampletype_t type, void* format, size_t formatSize) = 0;

	public:
		virtual error_t encode (size_t streamIndex, media_sample* sample) = 0;

	public:
		virtual error_t commit () = 0;
	};

	// Media Encoder Options
	struct DSEEDEXP media_encoder_options
	{
		size_t options_size;
	protected:
		media_encoder_options (size_t options_size) : options_size (options_size) { }
	};

	using createmediaencoder_fn = error_t (*) (dseed::stream*, const media_encoder_options*, media_encoder**);

	// Audio Stream
	class DSEEDEXP audio_stream : public dseed::stream
	{
	public:
		virtual error_t audioformat (audioformat* wf) = 0;
	};

	// Create Buffered Audio Stream
	DSEEDEXP error_t create_audio_buffered_stream (media_decoder* decoder, audio_stream** stream, int32_t streamIndex = -1);
	// Create Whole-Stored Audio Stream
	DSEEDEXP error_t create_audio_wholestored_stream (media_decoder* decoder, audio_stream** stream, int32_t streamIndex = -1);

	// Quad-Filter
	struct DSEEDEXP quadfilter
	{
		double a0, a1, a2, a3, a4;
		double x1[12], x2[12], y1[12], y2[12];

		float process (float input, int ch);

		static quadfilter high_pass_filter (int samplerate, double freq, double q);
		static quadfilter high_shelf_filter (int samplerate, double freq, float shelfSlope, double gaindb);
		static quadfilter low_pass_filter (int samplerate, double freq, double q);
		static quadfilter low_shelf_filter (int samplerate, double freq, float shelfSlope, double gaindb);
		static quadfilter all_pass_filter (int samplerate, double freq, double q);
		static quadfilter notch_filter (int samplerate, double freq, double q);
		static quadfilter peak_eq_filter (int samplerate, double freq, double bandwidth, double peakGainDB);
		static quadfilter band_pass_filter_csg (int samplerate, double freq, double q);
		static quadfilter band_pass_filter_cpg (int samplerate, double freq, double q);
	};

	// Equalizer Presets
	enum DSEEDEXP eqpreset_t
	{
		eqpreset_none,
		eqpreset_acoustic,
		eqpreset_dance,
		eqpreset_jazz,
		eqpreset_electronic,
		eqpreset_pop,
		eqpreset_rock,
		eqpreset_hiphop,
		eqpreset_classical,
		eqpreset_piano,
	};

	DSEEDEXP error_t initialize_equalizer_filter (uint32_t samplerate, double bandwidth, eqpreset_t preset, quadfilter* filters, size_t filtersCount);
	DSEEDEXP error_t initialize_equalizer_filter_with_gaindbs (uint32_t samplerate, double bandwidth,
		quadfilter* filters, double* gainDbs, size_t filtersAndGainDbsCount);

	DSEEDEXP error_t create_audio_filter_stream (audio_stream* original, quadfilter* filters, size_t filtersCount, audio_stream** stream);

	// Spectrum Processing Channels
	enum DSEEDEXP spectrumchannels_t : uint32_t
	{
		spectrumchannels_none = 0,
		spectrumchannels_ch1 = 1 << 0,
		spectrumchannels_ch2 = 1 << 1,
		spectrumchannels_ch3 = 1 << 2,
		spectrumchannels_ch4 = 1 << 3,
		spectrumchannels_ch5 = 1 << 4,
		spectrumchannels_ch6 = 1 << 5,
		spectrumchannels_ch7 = 1 << 6,
		spectrumchannels_ch8 = 1 << 7,
		spectrumchannels_ch9 = 1 << 8,
		spectrumchannels_ch10 = 1 << 9,
		spectrumchannels_average_all = 0xffffffff,
	};

	DSEEDEXP error_t convert_pcm_to_frequency (const float* pcm, size_t pcmSize, int16_t channels, spectrumchannels_t ch, float* out);
	DSEEDEXP error_t convert_pcm_to_decibel (const float* pcm, size_t pcmSize, int16_t channels, spectrumchannels_t ch, float* out);

	DSEEDEXP error_t reformat_audio (audio_stream* original, pulseformat_t fmt, int bits_per_sample, audio_stream** stream);

	enum resample_t
	{
		resample_nearest,
		resample_linear,
		resample_cubic,
		resample_lanczos1,
		resample_lanczos2,
		resample_lanczos3,
		resample_lanczos4,
		resample_lanczos5,
	};

	DSEEDEXP error_t resample_audio (audio_stream* original, int32_t samplerate, resample_t interpolation, audio_stream** stream);

	DSEEDEXP error_t rechannel_audio (audio_stream* original, int16_t channels, audio_stream** stream);
}

#include <dseed/media/media.decoders.h>
#include <dseed/media/media.encoders.h>

#endif