#include "Settings.h"

Daramee_Degra::WebPSettings::WebPSettings ( int quality )
{
	if ( quality < 1 || quality > 100 )
		throw ref new Platform::InvalidArgumentException ( L"Quality value must 1-100." );

	this->quality = quality;
}

Daramee_Degra::JpegSettings::JpegSettings ( int quality )
{
	if ( quality < 1 || quality > 100 )
		throw ref new Platform::InvalidArgumentException ( L"Quality value must 1-100." );

	this->quality = quality;
}

Daramee_Degra::PngSettings::PngSettings ( bool indexed )
{
	this->indexed = indexed;
}