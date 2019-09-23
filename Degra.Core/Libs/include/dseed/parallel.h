#ifndef __DSEED_PARALLEL_H__
#define __DSEED_PARALLEL_H__

#include <algorithm>

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <future>
#include <queue>

namespace dseed
{
	// Exclusive Mutex
	using mutex = std::mutex;
	// Exclusive/Shared Mutex
	using rwmutex = std::shared_timed_mutex;

	// Exclusive/Shared Spin-Lock
	struct spinlock
	{
	public:
		inline spinlock () noexcept : _sharedCount (0), _exclusiveFlag (false) { }

	public:
		inline void lock () noexcept
		{
			bool expected = false;
			while (!_exclusiveFlag.compare_exchange_weak (expected, true, std::memory_order_acquire))
				std::this_thread::yield ();

			while (_sharedCount.load (std::memory_order_seq_cst) != 0)
				std::this_thread::yield ();
		}
		inline void lock_shared () noexcept
		{
			while (true) {
				while (_exclusiveFlag.load (std::memory_order_seq_cst))
					std::this_thread::yield ();

				_sharedCount.fetch_add (1, std::memory_order_acquire);

				if (_exclusiveFlag.load (std::memory_order_seq_cst))
					_sharedCount.fetch_sub (1, std::memory_order_release);
				else
					break;
			}
		}

	public:
		inline bool try_lock () noexcept
		{
			bool expected = false;
			if (!_exclusiveFlag.load (std::memory_order_seq_cst) && _sharedCount.load (std::memory_order_seq_cst) == 0)
				return _exclusiveFlag.compare_exchange_weak (expected, true, std::memory_order_acquire);
			return false;
		}

		inline bool try_lock_shared () noexcept
		{
			if (!_exclusiveFlag.load (std::memory_order_seq_cst))
			{
				_sharedCount.fetch_add (1, std::memory_order_acquire);
				if (_exclusiveFlag.load (std::memory_order_seq_cst))
				{
					_sharedCount.fetch_sub (1, std::memory_order_release);
					return false;
				}
				return true;
			}
			return false;
		}

	public:
		inline void unlock () noexcept { _exclusiveFlag.store (false, std::memory_order_release); }
		inline void unlock_shared () noexcept { _sharedCount.fetch_sub (1, std::memory_order_release); }

	private:
		std::atomic<uint64_t> _sharedCount;
		std::atomic<bool> _exclusiveFlag;
	};

	// Thread-Pool
	template<class TLock = mutex>
	struct threadpool
	{
	public:
		inline threadpool (size_t maximum_threads = 0)
			: _stop (false), _running (0)
		{
			_workers.reserve (maximum_threads);
			for (size_t i = 0; i < maximum_threads; ++i)
			{
				_workers.emplace_back ([this] {
					while (true)
					{
						std::function<void ()> task;
						{
							std::unique_lock<TLock> lock (this->_mutex);

							this->_condition.wait (lock, [this] { return this->_stop || !this->_tasks.empty (); });
							if (this->_stop || this->_tasks.empty ())
								break;

							++_running;
							task = std::move (this->_tasks.front ());
							this->_tasks.pop ();
						}
						task ();

						--_running;
						_waitCondition.notify_all ();
					}
					});
			}
		}
		inline ~threadpool ()
		{
			{
				std::unique_lock<TLock> lock (_mutex);
				_stop = true;
			}
			_condition.notify_all ();
			for (std::thread& worker : _workers)
				worker.join ();
		}

	public:
		template<class F, class... Args>
		inline auto enqueue (F&& f, Args&& ... args)
		{
			using return_type = typename std::result_of<F (Args...)>::type;

			auto task = std::make_shared< std::packaged_task<return_type ()> > (
				std::bind (std::forward<F> (f), std::forward<Args> (args)...)
				);

			std::future<return_type> res = task->get_future ();
			{
				std::unique_lock<TLock> lock (_mutex);

				if (_stop)
					throw std::runtime_error ("enqueue on stopped Thread-Pool");

				_tasks.emplace ([task]() { (*task)(); });
			}
			_condition.notify_one ();
			return res;
		}

	public:
		inline void cancel_all ()
		{
			std::unique_lock<TLock> lock (_mutex);
			while (!_tasks.empty ())
				_tasks.pop ();
		}
		inline void wait ()
		{
			_waitCondition.wait (_mutex, [this]() -> bool { return _running == 0 && _tasks.empty (); });
		}

	private:
		std::vector<std::thread> _workers;
		std::queue<std::function<void ()>> _tasks;

		TLock _mutex;
		std::condition_variable _condition, _waitCondition;
		std::atomic<uint32_t> _running;

		bool _stop;
	};

	template<class TLock = mutex, size_t maximum_threads = 0>
	class parallel
	{
	public:
		inline parallel () : _threadpool (maximum_threads) { }

	public:
		template<class T>
		inline void for_each (T start, T end, const std::function<void (T)>& body) noexcept
		{
			threadpool<TLock> threadpool;
			for (T i = start; i != end; ++i)
				threadpool.enqueue (body, i);
			threadpool.wait ();
		}
		template<class T>
		inline void for_each_shared (T start, T end, const std::function<void (T)>& body) noexcept
		{
			for (T i = start; i != end; ++i)
				_threadpool.enqueue (body, i);
			_threadpool.wait ();
		}

	private:
		threadpool<TLock> _threadpool;
	};
}

#endif