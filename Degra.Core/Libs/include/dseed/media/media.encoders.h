#ifndef __DSEED_MEDIA_ENCODERS_H__
#define __DSEED_MEDIA_ENCODERS_H__

namespace dseed
{
	DSEEDEXP dseed::error_t create_wav_media_encoder (dseed::stream* stream, const dseed::media_encoder_options* options, dseed::media_encoder** encoder);
}

#endif