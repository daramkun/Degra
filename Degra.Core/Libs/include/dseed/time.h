#ifndef __DSEED_TIME_H__
#define __DSEED_TIME_H__

namespace dseed
{
	struct DSEEDEXP timespan_t
	{
	public:
		static timespan_t current_ticks () noexcept;

	public:
		timespan_t (int64_t ticks = 0) noexcept;
		timespan_t (int32_t hours, int32_t minutes, int32_t seconds, int32_t millisecs) noexcept;

	public:
		int64_t ticks () const noexcept;
		operator int64_t () const noexcept;
		int32_t milliseconds () const noexcept;
		int32_t seconds () const noexcept;
		int32_t minutes () const noexcept;
		int32_t hours () const noexcept;
		int32_t days () const noexcept;

	public:
		double total_milliseconds () const noexcept;
		double total_seconds () const noexcept;
		double total_minutes () const noexcept;
		double total_hours () const noexcept;
		double total_days () const noexcept;

	public:
		static timespan_t from_milliseconds (double millisecs) noexcept;
		static timespan_t from_seconds (double seconds) noexcept;
		static timespan_t from_minutes (double minutes) noexcept;
		static timespan_t from_hours (double hours) noexcept;
		static timespan_t from_days (double days) noexcept;

	public:
		timespan_t operator+ (const timespan_t& t) const { return _ticks + t._ticks; }
		timespan_t operator- (const timespan_t& t) const { return _ticks - t._ticks; }
		timespan_t operator+= (const timespan_t& t) { return _ticks += t._ticks; }
		timespan_t operator-= (const timespan_t& t) { return _ticks -= t._ticks; }

	private:
		int64_t _ticks;
	};

	struct DSEEDEXP frameskipper
	{
	public:
		frameskipper (int32_t framerate = 60);

	public:
		bool is_avaliable () noexcept;
		void reset_skipper_time () noexcept;
		timespan_t delta_time () const noexcept;

	public:
		void calculate () noexcept;
		timespan_t calced_framerate () const noexcept;

	public:
		timespan_t _last_time, _cur_time, _recommend_framerate, _framerate_checktime, _calculated_framerate;
		int32_t _framerate;
	};

	struct DSEEDEXP stopwatch
	{
	public:
		stopwatch ();

	public:
		void start () noexcept;
		void stop () noexcept;

	public:
		bool is_started () const noexcept;
		timespan_t elapsed () const noexcept;

	public:
		timespan_t _startedFrom;
		bool _isStarted;
	};
}

#endif