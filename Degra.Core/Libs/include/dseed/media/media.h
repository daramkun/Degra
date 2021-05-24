#ifndef __DSEED_MEDIA_H__
#define __DSEED_MEDIA_H__

namespace dseed::media
{
	using sampletype = uint32_t;
	enum
	{
		sampletype_unknown = 0,

		sampletype_image = 0x00000001,
		sampletype_audio = 0x00000002,

		sampletype_decoded = 0x00010000,
		sampletype_encoded = 0x00020000,

		sampletype_out_of_range = 0xffffffff,
	};

	// Image Format
	struct DSEEDEXP imageformat
	{
		size2i size;
		color::pixelformat image_format;
		fraction frame_rate;

		imageformat() = default;
		imageformat(const size2i& size, color::pixelformat format, fraction framerate)
			: size(size), image_format(format), frame_rate(framerate)
		{ }
	};

	// Audio Pulse Format
	enum class pulseformat : int32_t
	{
		// Unknown Format
		unknown,

		// Integer PCM Format
		pcm,
		// IEEE Float PCM Format
		ieee_float,
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
		pulseformat pulse_format;

		audioformat() = default;
		audioformat(int channels, int bits_per_sample, int samples_per_sec, pulseformat format)
			: channels(channels), bits_per_sample(bits_per_sample), samples_per_sec(samples_per_sec)
			, pulse_format(format)
			, block_align(channels* (bits_per_sample / 8)), bytes_per_sec(block_align* samples_per_sec)
		{ }
	};

	// Get Audio Duration from Bytes
	DSEEDEXP timespan get_audio_duration(const audioformat& wf, size_t bytes);
	// Get Bytes Count from Duration
	DSEEDEXP size_t get_audio_bytes_count(const audioformat& wf, timespan duration);
	// Get Bytes Count from Duration
	DSEEDEXP size_t get_audio_bytes_count(uint32_t bytes_per_sec, timespan duration);

	// Media Sample
	class DSEEDEXP media_sample : public dseed::object
	{
	public:
		virtual sampletype type() = 0;

	public:
		// Get Sample Position
		virtual timespan position() = 0;
		// Get Sample Length
		virtual timespan duration() = 0;

	public:
		// Copy Sample Data to buffer
		virtual dseed::error_t copy_sample(void* buffer) = 0;
		// Get Sample BUffer Size
		virtual size_t sample_size() = 0;
	};

	// Media Decoder
	class DSEEDEXP media_decoder : public dseed::object
	{
	public:
		// Get Total Duration
		virtual timespan duration() = 0;

	public:
		virtual sampletype type(size_t streamIndex) = 0;

	public:
		virtual error_t format(size_t streamIndex, void* buffer, size_t bufferSize) = 0;

	public:
		// Get Reading Position
		virtual timespan reading_position() = 0;
		// Set Reading Position
		virtual error_t set_reading_position(timespan pos) = 0;

	public:
		virtual error_t decode(size_t streamIndex, media_sample** sample) = 0;
	};

	// Media Encoder
	class DSEEDEXP media_encoder : public dseed::object
	{
	public:
		virtual error_t set_sample_format(size_t streamIndex, sampletype type, void* format, size_t formatSize) = 0;

	public:
		virtual error_t encode(size_t streamIndex, media_sample* sample) = 0;

	public:
		virtual error_t commit() = 0;
	};

	// Media Encoder Options
	struct DSEEDEXP media_encoder_options
	{
		size_t options_size;
	protected:
		media_encoder_options(size_t options_size) : options_size(options_size) { }
	};

	using createmediaencoder_fn = error_t(*) (dseed::io::stream*, const media_encoder_options*, media_encoder**);

	// Audio Stream
	class DSEEDEXP audio_stream : public dseed::io::stream
	{
	public:
		virtual error_t format(audioformat* wf) = 0;
	};

	// Create Buffered Audio Stream
	DSEEDEXP error_t create_audio_buffered_stream(media_decoder* decoder, audio_stream** stream, int32_t streamIndex = -1);
	// Create Whole-Stored Audio Stream
	DSEEDEXP error_t create_audio_wholestored_stream(media_decoder* decoder, audio_stream** stream, int32_t streamIndex = -1);

	// Quad-Filter
	struct DSEEDEXP quadfilter
	{
		double a0, a1, a2, a3, a4;
		double x1[12], x2[12], y1[12], y2[12];

		float process(float input, int ch);

		static quadfilter high_pass_filter(int samplerate, double freq, double q);
		static quadfilter high_shelf_filter(int samplerate, double freq, float shelfSlope, double gaindb);
		static quadfilter low_pass_filter(int samplerate, double freq, double q);
		static quadfilter low_shelf_filter(int samplerate, double freq, float shelfSlope, double gaindb);
		static quadfilter all_pass_filter(int samplerate, double freq, double q);
		static quadfilter notch_filter(int samplerate, double freq, double q);
		static quadfilter peak_eq_filter(int samplerate, double freq, double bandwidth, double peakGainDB);
		static quadfilter band_pass_filter_csg(int samplerate, double freq, double q);
		static quadfilter band_pass_filter_cpg(int samplerate, double freq, double q);
	};

	// Equalizer Presets
	enum class eqpreset
	{
		none,
		acoustic,
		dance,
		jazz,
		electronic,
		pop,
		rock,
		hiphop,
		classical,
		piano,
	};

	DSEEDEXP error_t initialize_equalizer_filter(uint32_t samplerate, double bandwidth, eqpreset preset, quadfilter* filters, size_t filtersCount);
	DSEEDEXP error_t initialize_equalizer_filter_with_gaindbs(uint32_t samplerate, double bandwidth,
		quadfilter* filters, double* gainDbs, size_t filtersAndGainDbsCount);

	DSEEDEXP error_t create_audio_filter_stream(audio_stream* original, quadfilter* filters, size_t filtersCount, audio_stream** stream);

	// Spectrum Processing Channels
	enum DSEEDEXP spectrumchannels : uint32_t
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

	DSEEDEXP error_t convert_pcm_to_frequency(const float* pcm, size_t pcmSize, int16_t channels, spectrumchannels ch, float* out);
	DSEEDEXP error_t convert_pcm_to_decibel(const float* pcm, size_t pcmSize, int16_t channels, spectrumchannels ch, float* out);

	DSEEDEXP error_t reformat_audio(audio_stream* original, pulseformat fmt, int bits_per_sample, audio_stream** stream);

	enum class resample
	{
		nearest,
		linear,
		cubic,
		lanczos1,
		lanczos2,
		lanczos3,
		lanczos4,
		lanczos5,
	};

	DSEEDEXP error_t resample_audio(audio_stream* original, int32_t samplerate, resample interpolation, audio_stream** stream);

	//DSEEDEXP error_t rechannel_audio (audio_stream* original, int16_t channels, audio_stream** stream);
}

#include "decoders.h"
#include "encoders.h"

namespace dseed::media
{
	using createmediadecoder_fn = error_t(*) (dseed::io::stream*, media_decoder**);
	DSEEDEXP error_t add_media_decoder(createmediadecoder_fn fn);
	DSEEDEXP error_t detect_media_decoder(dseed::io::stream* stream, dseed::media::media_decoder** decoder);
}

#endif