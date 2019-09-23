#ifndef __DSEED_OBJECT_H__
#define __DSEED_OBJECT_H__

#include <memory>

namespace dseed
{
	// Singleton Template Object
	template<typename T>
	class DSEEDEXP singleton
	{
	protected:
		singleton () { }
		virtual ~singleton () { }

	public:
		static T* instance () noexcept
		{
			if (_instance == nullptr)
				_instance = new T;
			return _instance;
		}

	private:
		static std::shared_ptr<T> _instance;
	};
	template<typename T> std::shared_ptr<T> singleton<T>::_instance = nullptr;

	// Native Object Wrapped Object
	class DSEEDEXP wrapped
	{
	public:
		virtual ~wrapped ();

	public:
		virtual error_t native_object (void** nativeObject) = 0;
	};

	// DSeed Base Reference Counting Object
	class DSEEDEXP object
	{
	protected:
		object ();

	public:
		virtual ~object ();

	public:
		virtual int32_t retain () = 0;
		virtual int32_t release () = 0;

	private:
		char _signature[8];
	};

	// Auto Reference Counting Container Object
	template<typename T>
	struct auto_object
	{
	public:
		inline auto_object () : auto_object (nullptr) { }
		inline auto_object (T* obj)
			: _ptr (obj)
		{
			static_assert (std::is_base_of<dseed::object, T>::value, "Not supported type.");

			if (_ptr)
				_ptr->retain ();
		}
		inline auto_object (const auto_object<T>& obj)
			: auto_object (obj.get ())
		{

		}
		inline ~auto_object () { release (); }

	public:
		inline void store (T* obj)
		{
			release ();
			if (_ptr = obj)
				_ptr->retain ();
		}
		inline void release ()
		{
			if (_ptr)
			{
				_ptr->release ();
				_ptr = nullptr;
			}
		}

		inline void attach (T* obj)
		{
			release ();
			_ptr = obj;
		}
		inline T* detach ()
		{
			T* ret = dynamic_cast<T*> (_ptr);
			_ptr = nullptr;
			return ret;
		}
		inline T* get () const { return dynamic_cast<T*> (_ptr); }

	public:
		inline T** operator & () { return reinterpret_cast<T * *> (&_ptr); }
		inline T* operator -> () const { return dynamic_cast<T*> (_ptr); }
		inline operator T* () const { return dynamic_cast<T*> (_ptr); }
		//inline explicit operator object* () const { return _ptr; }
		inline auto_object<T>& operator = (T* obj)
		{
			store (obj);
			return *this;
		}

	private:
		dseed::object* _ptr;
	};
}

#endif