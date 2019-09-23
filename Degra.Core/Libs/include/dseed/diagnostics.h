#ifndef __DSEED_DIAGNOSTICS_H__
#define __DSEED_DIAGNOSTICS_H__

namespace dseed
{
	enum loglevel_t
	{
		loglevel_debug_only,
		loglevel_information,
		loglevel_warning,
		loglevel_error,
		loglevel_fatal,
	};

	class DSEEDEXP logwriter : public object
	{
	public:
		virtual void write (loglevel_t level, const char* tag, const char* message) = 0;
	};

	DSEEDEXP error_t create_stream_logwriter (dseed::stream* stream, dseed::logwriter** writer);

	class DSEEDEXP logger
	{
	protected:
		logger () = default;

	public:
		virtual ~logger () = default;

	public:
		virtual void register_writer (logwriter* writer) = 0;
		virtual void unregister_writer (logwriter* writer) = 0;

	public:
		virtual void set_minimum_print_level (loglevel_t logLevel) = 0;
		virtual void set_maximum_print_level (loglevel_t logLevel) = 0;

	public:
		virtual void write (loglevel_t level, const char* tag, const char* message) = 0;

	public:
		static logger* shared_logger ();
	};
}

#endif