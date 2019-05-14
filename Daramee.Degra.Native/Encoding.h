#ifndef __DARAMEE_DEGRA_ENCODING_H__
#define __DARAMEE_DEGRA_ENCODING_H__

#include <wincodec.h>

void Encode_WebP ( IStream* stream, IWICBitmapSource* source, int quality, bool lossless );
void Encode_WIC_Jpeg ( IWICImagingFactory* wicFactory, IStream* stream, IWICBitmapSource* source, int quality );
void Encode_WIC_PNG ( IWICImagingFactory* wicFactory, IStream* stream, IWICBitmapSource* source );
void Encode_MozJpeg_Jpeg ( IStream* stream, IWICBitmapSource* source, int quality );
void Encode_Zopfli_PNG ( IWICImagingFactory* wicFactory, IStream* stream, IWICBitmapSource* source );


#endif