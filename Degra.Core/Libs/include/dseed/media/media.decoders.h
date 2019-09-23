#ifndef __DSEED_MEDIA_DECODERS_H__
#define __DSEED_MEDIA_DECODERS_H__

namespace dseed
{
	DSEEDEXP error_t create_media_foundation_media_decoder (dseed::stream* stream, dseed::media_decoder** decoder);

	DSEEDEXP error_t create_wav_media_decoder (dseed::stream* stream, dseed::media_decoder** decoder);
	DSEEDEXP error_t create_mp3_media_decoder (dseed::stream* stream, dseed::media_decoder** decoder);

	DSEEDEXP error_t create_vorbis_media_decoder (dseed::stream* stream, dseed::media_decoder** decoder);
	DSEEDEXP error_t create_opus_media_decoder (dseed::stream* stream, dseed::media_decoder** decoder);
	DSEEDEXP error_t create_flac_media_decoder (dseed::stream* stream, dseed::media_decoder** decoder);
}

#endif