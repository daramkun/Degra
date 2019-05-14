#ifndef __DARAMEE_DEGRA_SETTINGS_H__
#define __DARAMEE_DEGRA_SETTINGS_H__

namespace Daramee_Degra
{
	public interface class IEncodingSettings
	{
	public:
		virtual property Platform::String^ Extension
		{
			Platform::String^ get () = 0;
		}
	};

	public ref class WebPSettings sealed : public IEncodingSettings
	{
	public:
		WebPSettings ( int quality, bool lossless );

	public:
		virtual property Platform::String^ Extension { Platform::String^ get () { return L".webp"; } }

	public:
		property int Quality
		{
			int get () { return quality; }
		};
		property bool LosslessCompression
		{
			bool get () { return lossless; }
		};

	private:
		int quality;
		bool lossless;
	};

	public ref class JpegSettings sealed : public IEncodingSettings
	{
	public:
		JpegSettings ( int quality );

	public:
		virtual property Platform::String^ Extension { Platform::String^ get () { return L".jpg"; } }

	public:
		property int Quality
		{
			int get () { return quality; }
		};

	private:
		int quality;
	};

	public ref class PngSettings sealed : public IEncodingSettings
	{
	public:
		PngSettings ( bool indexed, bool zopfli );

	public:
		virtual property Platform::String^ Extension { Platform::String^ get () { return L".png"; } }

	public:
		property bool Indexed
		{
			bool get () { return indexed; }
		}
		property bool UseZopfli
		{
			bool get () { return zopfli; }
		}

	private:
		bool indexed, zopfli;
	};
}

#endif