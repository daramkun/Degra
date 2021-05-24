#ifndef __DSEED_OBJECT_H__
#define __DSEED_OBJECT_H__

namespace dseed
{
	class DSEEDEXP object
	{
	protected:
		object();

	public:
		virtual ~object();

	public:
		virtual int32_t retain() = 0;
		virtual int32_t release() = 0;

	private:
		char _signature[8];
	};

	class DSEEDEXP wrapped
	{
	public:
		virtual ~wrapped();

	public:
		virtual error_t native_object(void** nativeObject) = 0;
	};

	template<typename T>
	struct autoref
	{
	public:
		inline autoref() : autoref(nullptr) { }
		inline autoref(T* obj)
			: _ptr(obj)
		{
			static_assert (std::is_base_of<dseed::object, T>::value, "Not supported type.");

			if (_ptr)
				_ptr->retain();
		}
		inline autoref(const autoref<T>& obj)
			: _ptr(nullptr)
		{
			store(obj.get());
		}
		inline ~autoref() { release(); }

	public:
		inline void store(T* obj)
		{
			release();
			if ((_ptr = obj) != nullptr)
				_ptr->retain();
		}
		inline void release()
		{
			if (_ptr
#if PLATFORM_MICROSOFT
#	if ARCH_IA32 || ARCH_ARM
				&& (_ptr != (void*)0xcccccccc && _ptr != (void*)0xcdcdcdcd)
#	else
				&& (_ptr != reinterpret_cast<void*>(0xcccccccccccccccc) && _ptr != reinterpret_cast<void*>(0xcdcdcdcdcdcdcdcd))
#	endif
#endif
				)
			{
				_ptr->release();
				_ptr = nullptr;
			}
		}

		inline void attach(T* obj)
		{
			release();
			_ptr = obj;
		}
		inline T* detach()
		{
			T* ret = dynamic_cast<T*> (_ptr);
			_ptr = nullptr;
			return ret;
		}
		inline T* get() const { return dynamic_cast<T*> (_ptr); }

	public:
		inline T** operator & () { return reinterpret_cast<T**> (&_ptr); }
		inline T* operator -> () const { return dynamic_cast<T*> (_ptr); }
		inline operator T* () const { return dynamic_cast<T*> (_ptr); }
		inline autoref<T>& operator= (T* obj)
		{
			store(obj);
			return *this;
		}
		inline autoref<T>& operator= (const autoref<T>& obj)
		{
			store(obj.get());
			return *this;
		}

	private:
		dseed::object* _ptr;
	};

	class DSEEDEXP blob : public object
	{
	public:
		virtual void* ptr() noexcept = 0;
		virtual const void* ptr() const noexcept = 0;
		virtual size_t size() const noexcept = 0;
	};

	DSEEDEXP error_t create_buffered_blob(const void* data, size_t length, dseed::blob** blob) noexcept;
	DSEEDEXP error_t create_empty_blob(size_t length, dseed::blob** blob) noexcept;
	DSEEDEXP error_t create_wrapped_blob(void* data, size_t length, dseed::blob** blob) noexcept;

	class DSEEDEXP attributes : public object
	{
	public:
		virtual error_t get_int32(attrkey_t key, int32_t* value) = 0;
		virtual error_t get_uint32(attrkey_t key, uint32_t* value) = 0;
		virtual error_t get_int64(attrkey_t key, int64_t* value) = 0;
		virtual error_t get_uint64(attrkey_t key, uint64_t* value) = 0;
		virtual error_t get_single(attrkey_t key, float* value) = 0;
		virtual error_t get_double(attrkey_t key, double* value) = 0;
		virtual error_t get_object(attrkey_t key, object** value) = 0;

	public:
		virtual error_t set_int32(attrkey_t key, int32_t value) = 0;
		virtual error_t set_uint32(attrkey_t key, uint32_t value) = 0;
		virtual error_t set_int64(attrkey_t key, int64_t value) = 0;
		virtual error_t set_uint64(attrkey_t key, uint64_t value) = 0;
		virtual error_t set_single(attrkey_t key, float value) = 0;
		virtual error_t set_double(attrkey_t key, double value) = 0;
		virtual error_t set_object(attrkey_t key, object* value) = 0;

	public:
		virtual error_t get_size(attrkey_t key, uint32_t* width, uint32_t* height);
		virtual error_t set_size(attrkey_t key, uint32_t width, uint32_t height);

	public:
		virtual error_t get_fraction(attrkey_t key, int32_t* numerator, int32_t* denominator);
		virtual error_t set_fraction(attrkey_t key, int32_t numerator, int32_t denominator);

	public:
		virtual error_t get_string(attrkey_t key, char* strBuffer, size_t strBufferMaxSize);
		virtual error_t set_string(attrkey_t key, const char* str);

	public:
		virtual error_t get_u16string(attrkey_t key, char16_t* strBuffer, size_t strBufferMaxSize);
		virtual error_t set_u16string(attrkey_t key, const char16_t* str);

	public:
		virtual error_t get_struct(attrkey_t key, void* buffer, size_t bufferSize);
		virtual error_t set_struct(attrkey_t key, const void* data, size_t dataSize);
	};

	DSEEDEXP error_t create_attributes(attributes** attr) noexcept;
}

#include <queue>
namespace dseed
{
	class DSEEDEXP blobpool : public object
	{
	public:
		virtual error_t get_blob(size_t size, blob** b) = 0;
		virtual error_t return_blob(blob* b) = 0;
		virtual void clear_blobs() = 0;
	};

	DSEEDEXP error_t create_blobpool(blobpool** pool) noexcept;

	template<class T>
	class memorypool
	{
	public:
		inline ~memorypool() { clear_memories(); }

	public:
		inline T* get_memory()
		{
			if (pool.empty())
				return new T();
			else
			{
				T* mem = pool.front();
				pool.pop();
				return mem;
			}
		}
		inline void return_memory(T* mem)
		{
			pool.push(mem);
		}
		inline void clear_memories()
		{
			while (!pool.empty())
			{
				delete pool.front();
				pool.pop();
			}
		}

	private:
		std::queue<T*> pool;
	};
}

#endif