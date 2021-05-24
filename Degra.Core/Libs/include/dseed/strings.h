#ifndef __DSEED_STRINGS_H__
#define __DSEED_STRINGS_H__

#include <string>
#include <vector>

namespace dseed
{
	DSEEDEXP void utf8_to_utf16(const char* u8, char16_t* u16, size_t u16size) noexcept;
	DSEEDEXP void utf16_to_utf8(const char16_t* u16, char* u8, size_t u8size) noexcept;

	DSEEDEXP void utf16_to_wchar(const char16_t* u16, wchar_t* wc, size_t wcsize) noexcept;
	DSEEDEXP void wchar_to_utf16(const wchar_t* wc, char16_t* u16, size_t u16size) noexcept;

	inline std::u16string utf8_to_utf16(const std::string& u8) noexcept
	{
		std::vector<char16_t> temp(u8.length() * 4);
		utf8_to_utf16(u8.c_str(), temp.data(), temp.size());
		return temp.data();
	}
	inline std::string utf16_to_utf8(const std::u16string& u16) noexcept
	{
		std::vector<char> temp(u16.length());
		utf16_to_utf8(u16.c_str(), temp.data(), temp.size());
		return temp.data();
	}

	DSEEDEXP void path_combine(const char* path1, const char* path2, char* ret, size_t retsize) noexcept;
	DSEEDEXP void path_combine(const char16_t* path1, const char16_t* path2, char16_t* ret, size_t retsize) noexcept;
	DSEEDEXP void path_combine(const wchar_t* path1, const wchar_t* path2, wchar_t* ret, size_t retsize) noexcept;

	inline std::string path_combine(const std::string& path1, const std::string& path2) noexcept
	{
		char temp[512];
		path_combine(path1.c_str(), path2.c_str(), temp, 512);
		return temp;
	}
	inline std::u16string path_combine(const std::u16string& path1, const std::u16string& path2) noexcept
	{
		char16_t temp[512];
		path_combine(path1.c_str(), path2.c_str(), temp, 512);
		return temp;
	}
}

#endif