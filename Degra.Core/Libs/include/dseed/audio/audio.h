#ifndef __DSEED_AUDIO_H__
#define __DSEED_AUDIO_H__

namespace dseed::audio
{
	enum class audioadapter_type
	{
		speaker,
		listener,
	};

	class DSEEDEXP audioadapter : public object, public wrapped
	{
	public:
		virtual error_t name(char* name, size_t maxNameCount) noexcept = 0;
		virtual error_t info(dseed::media::audioformat* info) noexcept = 0;
		virtual audioadapter_type type() noexcept = 0;
	};

	class DSEEDEXP audioadapter_enumerator : public object
	{
	public:
		virtual error_t audioadapter(size_t index, audioadapter** adapter) noexcept = 0;
		virtual size_t audioadapter_count() noexcept = 0;
	};
}

namespace dseed::audio
{
	enum class audiostate
	{
		stopped,
		playing,
		paused,
	};

	struct audiolistener
	{
		f32x4_t forward;
		f32x4_t position;
		f32x4_t up;
		f32x4_t velocity;
	};

	struct audioemitter
	{
		f32x4_t forward;
		f32x4_t position;
		f32x4_t up;
		f32x4_t velocity;
		float dopplerScale;
	};

	class DSEEDEXP audiorequester : public object, public wrapped
	{
	public:
		virtual error_t format(dseed::media::audioformat* format) = 0;

	public:
		virtual error_t start() = 0;
		virtual error_t stop() = 0;

	public:
		virtual bool silent_null() = 0;
		virtual void set_silent_null(bool sn) = 0;

	public:
		virtual error_t request(void* buffer, size_t bufferMaxSize, size_t* dataSize) = 0;
	};

	class DSEEDEXP backgroundaudio : public object
	{
	public:
		virtual error_t format(dseed::media::audioformat* fmt) noexcept = 0;
		virtual audiostate state() noexcept = 0;

	public:
		virtual error_t play() noexcept = 0;
		virtual error_t stop() noexcept = 0;
		virtual error_t pause() noexcept = 0;

	public:
		virtual timespan position() noexcept = 0;
		virtual timespan duration() noexcept = 0;

	public:
		virtual float volume() noexcept = 0;
		virtual error_t set_volume(float vol) noexcept = 0;
	};

	using effectid_t = uint32_t;
	using instanceid_t = uint64_t;

	constexpr effectid_t invalid_effectid = 0xffffffff;
	constexpr instanceid_t invalid_instanceid = 0xffffffffffffffff;

	class DSEEDEXP effectaudio : public object
	{
	public:
		virtual error_t format(dseed::media::audioformat* fmt) noexcept = 0;

	public:
		virtual error_t load_effectid(dseed::media::audio_stream* stream, effectid_t* eid) noexcept = 0;
		virtual error_t unload_effectid(effectid_t eid) noexcept = 0;
		virtual error_t unload_all_effects() noexcept = 0;

	public:
		virtual error_t create_instance(effectid_t eid, instanceid_t* iid) noexcept = 0;
		virtual error_t release_instance(instanceid_t iid) noexcept = 0;
		virtual error_t release_all_instances() noexcept = 0;

	public:
		virtual error_t set_listener(instanceid_t iid, const audiolistener* listener) noexcept = 0;
		virtual error_t set_emitter(instanceid_t iid, const audioemitter* emitter) noexcept = 0;

	public:
		virtual error_t play(instanceid_t iid) noexcept = 0;
		virtual error_t stop(instanceid_t iid) noexcept = 0;

	public:
		virtual audiostate state(instanceid_t iid) noexcept = 0;

	public:
		virtual float volume() noexcept = 0;
		virtual error_t set_volume(float vol) noexcept = 0;
	};

	class DSEEDEXP audioplayer : public object, public wrapped
	{
	public:
		virtual float volume() noexcept = 0;
		virtual error_t set_volume(float vol) noexcept = 0;

	public:
		virtual error_t create_backgroundaudio(dseed::media::audio_stream* stream, backgroundaudio** audio) noexcept = 0;
		virtual error_t create_effectaudio(dseed::media::audioformat* format, effectaudio** audio) noexcept = 0;
	};
}

#if PLATFORM_MICROSOFT && defined(USE_XAUDIO2_NATIVE_OBJECT)
#	include <wrl.h>
#	include <xaudio2.h>
#endif
#if PLATFORM_MICROSOFT && defined(USE_WASAPI_NATIVE_OBJECT)
#	include <wrl.h>
#	include <mmdeviceapi.h>
#	include <Audioclient.h>
#endif

namespace dseed::audio
{
#if PLATFORM_MICROSOFT && defined(USE_XAUDIO2_NATIVE_OBJECT)
	struct xaudio2_audioplayer_nativeobject
	{
		Microsoft::WRL::ComPtr<IXAudio2> xaudio2;
		IXAudio2MasteringVoice* masteringVoice;
	};

	struct xaudio2_audioplaybuffer_nativeobject
	{
		IXAudio2SourceVoice* sourceVoice;
	};
#endif
#if PLATFORM_MICROSOFT && defined(USE_WASAPI_NATIVE_OBJECT)
	struct wasapi_audioadapter_nativeobject
	{
		Microsoft::WRL::ComPtr<IMMDevice> mmDevice;
	};

	struct wasapi_audiorequester_nativeobject
	{
		Microsoft::WRL::ComPtr<IAudioClient> audioClient;
		Microsoft::WRL::ComPtr<IAudioCaptureClient> audioCaptureClient;
	};
#endif
#if defined(USE_OPENAL_NATIVE_OBJECT)

#endif
}

namespace dseed::audio
{
	DSEEDEXP error_t create_wasapi_audiooadapter_enumerator(audioadapter_type type, audioadapter_enumerator** enumerator) noexcept;
	DSEEDEXP error_t create_xaudio2_audioplayer(audioadapter* adapter, audioplayer** player) noexcept;
	DSEEDEXP error_t create_wasapi_audiorequester(audioadapter* adapter, audiorequester** requester) noexcept;

	DSEEDEXP error_t create_openal_audioadapter_enumerator(audioadapter_type type, audioadapter_enumerator** enumerator) noexcept;
	DSEEDEXP error_t create_openal_audioplayer(audioadapter* adapter, audioplayer** player) noexcept;
	DSEEDEXP error_t create_openal_audiorequester(audioadapter* adapter, audiorequester** requester) noexcept;
}

#endif