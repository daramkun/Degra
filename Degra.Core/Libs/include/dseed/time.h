#ifndef __DSEED_TIME_H__
#define __DSEED_TIME_H__

namespace dseed
{
	struct DSEEDEXP timespan
	{
	public:
		static timespan current_ticks() noexcept;

	public:
		timespan(int64_t ticks = 0) noexcept;
		timespan(int32_t hours, int32_t minutes, int32_t seconds, int32_t millisecs) noexcept;

	public:
		int64_t ticks() const noexcept;
		operator int64_t () const noexcept;
		int32_t milliseconds() const noexcept;
		int32_t seconds() const noexcept;
		int32_t minutes() const noexcept;
		int32_t hours() const noexcept;
		int32_t days() const noexcept;

	public:
		double total_milliseconds() const noexcept;
		double total_seconds() const noexcept;
		double total_minutes() const noexcept;
		double total_hours() const noexcept;
		double total_days() const noexcept;

	public:
		static timespan from_milliseconds(double millisecs) noexcept;
		static timespan from_seconds(double seconds) noexcept;
		static timespan from_minutes(double minutes) noexcept;
		static timespan from_hours(double hours) noexcept;
		static timespan from_days(double days) noexcept;

	public:
		inline timespan operator+ (const timespan& t) const { return _ticks + t._ticks; }
		inline timespan operator- (const timespan& t) const { return _ticks - t._ticks; }
		inline timespan operator+= (const timespan& t) { return _ticks += t._ticks; }
		inline timespan operator-= (const timespan& t) { return _ticks -= t._ticks; }

	private:
		int64_t _ticks;
	};

	struct DSEEDEXP frameskipper
	{
	public:
		frameskipper(int32_t framerate = 60);

	public:
		bool is_avaliable() noexcept;
		void reset_skipper_time() noexcept;
		timespan delta_time() const noexcept;

	public:
		void calculate() noexcept;
		timespan calced_framerate() const noexcept;

	public:
		timespan _last_time, _cur_time, _recommend_framerate, _framerate_checktime, _calculated_framerate;
		int32_t _framerate;
	};

	struct DSEEDEXP stopwatch
	{
	public:
		stopwatch();

	public:
		void start() noexcept;
		void stop() noexcept;

	public:
		bool is_started() const noexcept;
		timespan elapsed() const noexcept;

	public:
		timespan _startedFrom;
		bool _isStarted;
	};

	struct DSEEDEXP framemeasurer
	{
	public:
		framemeasurer();

	public:
		void update(timespan delta) noexcept;

	public:
		double fps() const noexcept;

	public:
		timespan _elapsed;
		size_t _count;
		double _fps;
	};
}

#endif