#ifndef __DSEED_MEDIA_ENCODERS_H__
#define __DSEED_MEDIA_ENCODERS_H__

namespace dseed::media
{
	DSEEDEXP dseed::error_t create_wav_media_encoder(dseed::io::stream* stream, const dseed::media::media_encoder_options* options, dseed::media::media_encoder** encoder);
}

#endif