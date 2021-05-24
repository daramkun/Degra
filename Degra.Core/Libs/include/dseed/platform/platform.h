#ifndef __DSEED_PLATFORM_H__
#define __DSEED_PLATFORM_H__

namespace dseed::platform
{
	class DSEEDEXP event_handler : public object
	{
	public:
		virtual void activated() = 0;
		virtual void deactivated() = 0;

	public:
		virtual void paused() = 0;
		virtual void resumed() = 0;

	public:
		virtual void resized() = 0;

	public:
		virtual void shown() = 0;
		virtual void closing(bool& cancel) = 0;
		virtual void closed() = 0;

	public:
		virtual void next_frame(timespan delta) = 0;
	};

	enum class DSEEDEXP windowmode : int32_t
	{
		unknown,
		windowed,
		sizable_windowed,
		borderless_fullscreen,
	};

	class DSEEDEXP application : public object, public wrapped
	{
	public:
		virtual error_t input_device(dseed::io::inputdevicetype type, dseed::io::inputdevice** device) = 0;

	public:
		virtual error_t set_title(const char* title) = 0;

	public:
		virtual error_t client_size(size2i* size) = 0;
		virtual error_t set_client_size(const size2i& size) = 0;

	public:
		virtual error_t window_mode(windowmode* mode) = 0;
		virtual error_t set_window_mode(windowmode mode) = 0;

	public:
		virtual error_t dpi(size2i* dpi) = 0;

	public:
		virtual error_t event_handler(dseed::platform::event_handler** handler) = 0;
		virtual error_t set_event_handler(dseed::platform::event_handler* handler) = 0;

	public:
		virtual error_t run(dseed::platform::event_handler* handler) = 0;

	public:
		virtual error_t exit() = 0;

	public:
		static error_t shared_app(application** app);
	};

	DSEEDEXP error_t create_application(application** app);
}

#endif