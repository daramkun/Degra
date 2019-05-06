#pragma once

#include <wrl.h>
#include <collection.h>
#include <ppltasks.h>

#include <atlbase.h>
#include <wincodec.h>

namespace Daramee_Degra
{
	public interface class IEncodingSettings
	{
	public:

	};

	public ref class WebPSettings sealed : public IEncodingSettings
	{
	public:
		WebPSettings ( int quality );

	public:
		property int Quality
		{
			int get () { return quality; }
		};

	private:
		int quality;
	};

	public ref class JpegSettings sealed : public IEncodingSettings
	{
	public:
		JpegSettings ( int quality );

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
		PngSettings ( bool indexed );

	public:
		property bool Indexed
		{
			bool get () { return indexed; }
		}

	private:
		bool indexed;
	};

	public enum class SeekOrigin
	{
		Begin,
		Current,
		End,
	};

	public interface class IDegraStream
	{
	public:
		virtual int Read ( Platform::WriteOnlyArray<byte>^ buffer, int length ) = 0;
		virtual int Write ( const Platform::Array<byte>^ data, int length ) = 0;
		virtual int Seek ( SeekOrigin origin, int offset ) = 0;
		virtual void Flush () = 0;
		property int Length { int get (); };
	};

	public ref class Argument sealed
	{
	public:
		Argument ( IEncodingSettings^ settings, bool dither, bool resizeBicubic, unsigned int maximumHeight );

	public:
		property unsigned int MaximumHeight { unsigned int get () { return maximumHeight; } };
		property bool Dither { bool get () { return dither; } }
		property bool ResizeBicubic { bool get () { return resizeBicubic; } }
		property IEncodingSettings^ Settings { IEncodingSettings^ get () { return settings; } }

	private:
		unsigned int maximumHeight;
		bool dither, resizeBicubic;
		IEncodingSettings^ settings;
	};

	public ref class DegraCore sealed
	{
	public:
		DegraCore ();

	private:
		~DegraCore ();

	public:
		void ConvertImage ( IDegraStream^ destStream, IDegraStream^ srcStream, Argument^ argument );

	private:
		IWICImagingFactory2* wicFactory;
	};
}
