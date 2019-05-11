#include "IDegraStream.h"

Daramee_Degra::ImplementedIStream::ImplementedIStream ( IDegraStream^ stream ) : refCount ( 1 ), stream ( stream ) { }
Daramee_Degra::ImplementedIStream::~ImplementedIStream () { stream = nullptr; }

HRESULT __stdcall Daramee_Degra::ImplementedIStream::QueryInterface ( REFIID riid, void** ppvObject )
{
	if ( riid == __uuidof ( IStream ) || riid == __uuidof ( IUnknown )
		|| riid == __uuidof( ISequentialStream ) )
	{
		*ppvObject = this;
		AddRef ();
		return S_OK;
	}
	return E_NOINTERFACE;
}
ULONG __stdcall Daramee_Degra::ImplementedIStream::AddRef ( void ) { return InterlockedIncrement ( &refCount ); }
ULONG __stdcall Daramee_Degra::ImplementedIStream::Release ( void ) { auto ret = InterlockedDecrement ( &refCount ); if ( ret == 0 ) delete this; return ret; }

HRESULT __stdcall Daramee_Degra::ImplementedIStream::Read ( void* pv, ULONG cb, ULONG * pcbRead )
{
	auto arr = ref new Platform::Array<byte> ( ( byte* ) pv, cb );
	auto read = stream->Read ( arr, cb );
	memcpy ( pv, arr->Data, read );
	if ( pcbRead )
		* pcbRead = read;
	return S_OK;
}

HRESULT __stdcall Daramee_Degra::ImplementedIStream::Write ( const void* pv, ULONG cb, ULONG* pcbWritten )
{
	auto arr = ref new Platform::Array<byte> ( ( byte* ) pv, cb );
	auto written = stream->Write ( arr, cb );
	if ( pcbWritten )
		* pcbWritten = written;
	return S_OK;
}

HRESULT __stdcall Daramee_Degra::ImplementedIStream::Seek ( LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition )
{
	int pos = stream->Seek ( ( Daramee_Degra::SeekOrigin ) dwOrigin, ( int ) dlibMove.QuadPart );
	if ( plibNewPosition )
		plibNewPosition->QuadPart = ( ULONGLONG ) pos;
	return S_OK;
}

HRESULT __stdcall Daramee_Degra::ImplementedIStream::Stat ( STATSTG* pstatstg, DWORD grfStatFlag )
{
	memset ( pstatstg, 0, sizeof ( STATSTG ) );
	pstatstg->type = STGTY_STREAM;
	pstatstg->cbSize.QuadPart = stream->Length;
	pstatstg->grfMode = STGM_READ | STGM_WRITE;
	return S_OK;
}

HRESULT __stdcall Daramee_Degra::ImplementedIStream::SetSize ( ULARGE_INTEGER libNewSize ) { return E_NOTIMPL; }
HRESULT __stdcall Daramee_Degra::ImplementedIStream::CopyTo ( IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten ) { return E_NOTIMPL; }
HRESULT __stdcall Daramee_Degra::ImplementedIStream::Commit ( DWORD grfCommitFlags ) { stream->Flush (); return S_OK; }
HRESULT __stdcall Daramee_Degra::ImplementedIStream::Revert ( void ) { return E_NOTIMPL; }
HRESULT __stdcall Daramee_Degra::ImplementedIStream::LockRegion ( ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType ) { return E_NOTIMPL; }
HRESULT __stdcall Daramee_Degra::ImplementedIStream::UnlockRegion ( ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType ) { return E_NOTIMPL; }
HRESULT __stdcall Daramee_Degra::ImplementedIStream::Clone ( IStream** ppstm ) { return E_NOTIMPL; }
