#ifndef __DSEED_IO_STREAM_H__
#define __DSEED_IO_STREAM_H__

namespace dseed::io
{
	enum class seekorigin : int32_t
	{
		begin,
		current,
		end,
	};

	class DSEEDEXP stream : public dseed::object
	{
	public:
		virtual size_t read(void* buffer, size_t length) noexcept = 0;
		virtual size_t write(const void* data, size_t length) noexcept = 0;
		virtual bool seek(seekorigin origin, size_t offset) noexcept = 0;
		virtual void flush() noexcept = 0;
		virtual dseed::error_t set_length(size_t length) noexcept = 0;

	public:
		virtual size_t position() noexcept = 0;
		virtual size_t length() noexcept = 0;

	public:
		virtual bool readable() noexcept = 0;
		virtual bool writable() noexcept = 0;
		virtual bool seekable() noexcept = 0;
	};

	DSEEDEXP dseed::error_t create_fixed_memorystream(void* buffer, size_t length, dseed::io::stream** stream);
	DSEEDEXP dseed::error_t create_variable_memorystream(dseed::io::stream** stream, bool remove_after_read = false);
	DSEEDEXP dseed::error_t create_native_filestream(const char* path, bool create, dseed::io::stream** stream);
}

#endif