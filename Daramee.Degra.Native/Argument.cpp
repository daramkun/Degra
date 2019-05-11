#include "Argument.h"

Daramee_Degra::Argument::Argument ( IEncodingSettings^ settings, bool dither, bool resizeBicubic, unsigned int maximumHeight )
{
	this->dither = dither;
	this->resizeBicubic = resizeBicubic;
	this->settings = settings;
	this->maximumHeight = maximumHeight;
}