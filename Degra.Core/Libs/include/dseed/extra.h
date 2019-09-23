#ifndef __DSEED_EXTRA_H__
#define __DSEED_EXTRA_H__

namespace dseed
{
	// Memory Blob
	class DSEEDEXP blob : public object
	{
	public:
		virtual void* blob_pointer () = 0;
		virtual size_t blob_length () = 0;
	};

	// Create Empty Blob
	DSEEDEXP error_t create_memoryblob (size_t length, dseed::blob** blob);
	// Create Blob from Exists Buffer
	DSEEDEXP error_t create_bufferblob (void* buffer, size_t length, bool copy, dseed::blob** blob);
	// Create Blob from Stream
	DSEEDEXP error_t create_streamblob (dseed::stream* stream, dseed::blob** blob);

	using attrkey_t = uint64_t;

	// Attributes
	class DSEEDEXP attributes : public object
	{
	public:
		virtual error_t get_int32 (attrkey_t key, int32_t* value) = 0;
		virtual error_t get_uint32 (attrkey_t key, uint32_t* value) = 0;
		virtual error_t get_int64 (attrkey_t key, int64_t* value) = 0;
		virtual error_t get_uint64 (attrkey_t key, uint64_t* value) = 0;
		virtual error_t get_single (attrkey_t key, float* value) = 0;
		virtual error_t get_double (attrkey_t key, double* value) = 0;
		virtual error_t get_object (attrkey_t key, object** value) = 0;

	public:
		virtual error_t set_int32 (attrkey_t key, int32_t value) = 0;
		virtual error_t set_uint32 (attrkey_t key, uint32_t value) = 0;
		virtual error_t set_int64 (attrkey_t key, int64_t value) = 0;
		virtual error_t set_uint64 (attrkey_t key, uint64_t value) = 0;
		virtual error_t set_single (attrkey_t key, float value) = 0;
		virtual error_t set_double (attrkey_t key, double value) = 0;
		virtual error_t set_object (attrkey_t key, object* value) = 0;

	public:
		virtual error_t get_size (attrkey_t key, uint32_t* width, uint32_t* height);
		virtual error_t set_size (attrkey_t key, uint32_t width, uint32_t height);

	public:
		virtual error_t get_fraction (attrkey_t key, int32_t* numerator, int32_t* denominator);
		virtual error_t set_fraction (attrkey_t key, int32_t numerator, int32_t denominator);

	public:
		virtual error_t get_string (attrkey_t key, char* strBuffer, size_t strBufferMaxSize);
		virtual error_t set_string (attrkey_t key, const char* str);

	public:
		virtual error_t get_u16string (attrkey_t key, char16_t* strBuffer, size_t strBufferMaxSize);
		virtual error_t set_u16string (attrkey_t key, const char16_t* str);

	public:
		virtual error_t get_struct (attrkey_t key, void* buffer, size_t bufferSize);
		virtual error_t set_struct (attrkey_t key, const void* data, size_t dataSize);
	};

	// Create Empty Attributes
	DSEEDEXP error_t create_attributes (attributes** attr);

	constexpr attrkey_t attrkey_cursor_hotspot = 0x00000001;
}

#endif