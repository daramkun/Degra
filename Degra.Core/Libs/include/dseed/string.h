#ifndef __DSEED_STRING_H__
#define __DSEED_STRING_H__

#include <string>
#include <vector>

namespace dseed
{
	// Convert UTF-8 String to UTF-16
	DSEEDEXP void utf8_to_utf16 (const char* u8, char16_t* u16, size_t u16size);
	// Convert UTF-16 String to UTF-8
	DSEEDEXP void utf16_to_utf8 (const char16_t* u16, char* u8, size_t u8size);

	// Convert UTF-8 String to UTF-16 for Standard String Object
	inline std::u16string utf8_to_utf16 (const std::string& u8) noexcept
	{
		std::vector<char16_t> temp (u8.length () * 4);
		utf8_to_utf16 (u8.c_str (), temp.data (), temp.size ());
		return temp.data ();
	}
	// Convert UTF-16 String to UTF-8 for Standard String Object
	inline std::string utf16_to_utf8 (const std::u16string& u16) noexcept
	{
		std::vector<char> temp (u16.length ());
		utf16_to_utf8 (u16.c_str (), temp.data (), temp.size ());
		return temp.data ();
	}

	// UTF-8 Path String Combine
	DSEEDEXP void path_combine (const char* path1, const char* path2, char* ret, size_t retsize);
	// UTF-8 Path String Combine for Standard String Object
	inline std::string path_combine (const std::string& path1, const std::string& path2)
	{
		char temp[512];
		path_combine (path1.c_str (), path2.c_str (), temp, 512);
		return temp;
	}

	// UTF-16 Path String Combine
	DSEEDEXP void path_combine (const char16_t* path1, const char16_t* path2, char16_t* ret, size_t retsize);
	// UTF-16 Path String Combine for Standard String Object
	inline std::u16string path_combine (const std::u16string& path1, const std::u16string& path2)
	{
		char16_t temp[512];
		path_combine (path1.c_str (), path2.c_str (), temp, 512);
		return temp;
	}
}

#endif