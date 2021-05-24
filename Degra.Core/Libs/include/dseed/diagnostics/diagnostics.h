#ifndef __DSEED_DIAGNOSTICS_H__
#define __DSEED_DIAGNOSTICS_H__

namespace dseed::diagnostics
{
	enum class loglevel
	{
		debug_only,
		information,
		warning,
		error,
		fatal,
	};

	class DSEEDEXP logwriter : public object
	{
	public:
		virtual void write(loglevel level, const char* tag, const char* message) = 0;
	};

	DSEEDEXP error_t create_stream_logwriter(dseed::io::stream* stream, dseed::diagnostics::logwriter** writer);

	class DSEEDEXP logger
	{
	protected:
		logger() = default;

	public:
		virtual ~logger() = default;

	public:
		virtual void register_writer(logwriter* writer) = 0;
		virtual void unregister_writer(logwriter* writer) = 0;

	public:
		virtual void set_minimum_print_level(loglevel logLevel) = 0;
		virtual void set_maximum_print_level(loglevel logLevel) = 0;

	public:
		virtual void write(loglevel level, const char* tag, const char* message) = 0;

	public:
		static logger* shared_logger();
	};
}

#endif