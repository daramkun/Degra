#include "Argument.h"

Daramee_Degra::Argument::Argument ( IEncodingSettings^ settings, bool dither, bool resizeBicubic, bool deepCheckAlpha, unsigned int maximumHeight )
{
	this->dither = dither;
	this->resizeBicubic = resizeBicubic;
	this->deepCheckAlpha = deepCheckAlpha;
	this->settings = settings;
	this->maximumHeight = maximumHeight;
}