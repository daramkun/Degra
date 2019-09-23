#ifndef __DSEED_AUDIO_H__
#define __DSEED_AUDIO_H__

namespace dseed
{
	class DSEEDEXP audio_adapter : public object
	{
	public:
		virtual error_t name (char* name, size_t maxNameCount) = 0;
		virtual error_t info (audioformat* info) = 0;
	};

	class DSEEDEXP audio_adapter_enumerator : public object
	{
	public:
		virtual error_t audio_adapter (int index, audio_adapter** adapter) = 0;
		virtual size_t audio_adapter_count () = 0;
	};

	enum audiobufferstate_t
	{
		audiobufferstate_stopped,
		audiobufferstate_playing,
	};

	struct audiolistener
	{
		float3 forward;
		float3 position;
		float3 up;
		float3 velocity;
	};

	struct audioemitter
	{
		float3 forward;
		float3 position;
		float3 up;
		float3 velocity;
		float dopplerScale;
	};

	class DSEEDEXP audioplaybuffer : public object
	{
	public:
		virtual float volume () = 0;
		virtual error_t set_volume (float vol) = 0;

	public:
		virtual error_t play () = 0;
		virtual error_t stop () = 0;

	public:
		virtual error_t buffering (void* buffer, size_t bufferSize) = 0;
		virtual error_t wait_to_buffer_end () = 0;

	public:
		virtual error_t set_listener (const audiolistener* emitter) = 0;
		virtual error_t set_emitter (const audioemitter* emitter) = 0;
	};

	class DSEEDEXP audioplayer : public object
	{
	public:
		virtual float volume () = 0;
		virtual error_t set_volume (float vol) = 0;

	public:
		virtual error_t create_buffer (const audioformat* format, audioplaybuffer** buffer) = 0;
	};

	class DSEEDEXP audiorequester : public object
	{
	public:
		virtual error_t format (audioformat* format) = 0;

	public:
		virtual error_t start () = 0;
		virtual error_t stop () = 0;

	public:
		virtual error_t request (void* buffer, size_t bufferMaxSize) = 0;
	};
}

#endif