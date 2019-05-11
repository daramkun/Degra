#ifndef __DARAMEE_DEGRA_IDEGRASTREAM_H__
#define __DARAMEE_DEGRA_IDEGRASTREAM_H__

#include <wrl.h>
#include <ppl.h>
#include <ppltasks.h>

namespace Daramee_Degra
{
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

	class ImplementedIStream : public IStream
	{
	public:
		ImplementedIStream ( IDegraStream^ stream );
		~ImplementedIStream ();

	public:
		virtual HRESULT __stdcall QueryInterface ( REFIID riid, void** ppvObject ) override;
		virtual ULONG __stdcall AddRef ( void ) override;
		virtual ULONG __stdcall Release ( void ) override;

	public:
		virtual HRESULT __stdcall Read ( void* pv, ULONG cb, ULONG* pcbRead ) override;
		virtual HRESULT __stdcall Write ( const void* pv, ULONG cb, ULONG* pcbWritten ) override;
		virtual HRESULT __stdcall Seek ( LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition ) override;

		virtual HRESULT __stdcall Stat ( STATSTG* pstatstg, DWORD grfStatFlag ) override;

		virtual HRESULT __stdcall SetSize ( ULARGE_INTEGER libNewSize ) override;
		virtual HRESULT __stdcall CopyTo ( IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten ) override;
		virtual HRESULT __stdcall Commit ( DWORD grfCommitFlags ) override;
		virtual HRESULT __stdcall Revert ( void ) override;
		virtual HRESULT __stdcall LockRegion ( ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType ) override;
		virtual HRESULT __stdcall UnlockRegion ( ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType ) override;
		virtual HRESULT __stdcall Clone ( IStream** ppstm ) override;

	private:
		ULONG refCount;
		IDegraStream^ stream;
	};
}

#endif