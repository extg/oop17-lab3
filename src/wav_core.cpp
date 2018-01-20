#include <cstdio>
#include <cstring>
#include <cmath>

#include "wav_header.h"
#include "wav_core.h"


// TODO: Remove all 'magic' numbers
// TODO: Make the code more secure. Get rid of pointers (after creating a class, of course).


wav_errors_e read_header(const char *filename, wav_header_s *header_ptr)
{
    printf( ">>>> read_header( %s )\n", filename );
    null_header( header_ptr); // Fill header with zeroes.

    FILE* f = fopen( filename, "rb" );
    if ( !f ) {
        return IO_ERROR;
    }

    size_t blocks_read = fread( header_ptr, sizeof(wav_header_s), 1, f);
    if ( blocks_read != 1 ) {
        // can't read header, because the file is too small.
        return BAD_FORMAT;
    }

    fseek( f, 0, SEEK_END ); // seek to the end of the file
    size_t file_size = ftell( f ); // current position is a file size!
    fclose( f );

    if ( check_header( header_ptr, file_size ) != HEADER_OK ) {
        return BAD_FORMAT;
    } else {
        return WAV_OK;
    }
}

void print_info(const wav_header_s *header_ptr)
{
    printf( "-------------------------\n" );
    printf( " audioFormat   %u\n", header_ptr->audioFormat );
    printf( " numChannels   %u\n", header_ptr->numChannels );
    printf( " sampleRate    %u\n", header_ptr->sampleRate );
    printf( " bitsPerSample %u\n", header_ptr->bitsPerSample );
    printf( " byteRate      %u\n", header_ptr->byteRate );
    printf( " blockAlign    %u\n", header_ptr->blockAlign );
    printf( " chunkSize     %u\n", header_ptr->chunkSize );
    printf( " subchunk1Size %u\n", header_ptr->subchunk1Size );
    printf( " subchunk2Size %u\n", header_ptr->subchunk2Size );
    printf( "-------------------------\n" );
}


wav_errors_e extract_data_int16( const char* filename, std::vector<std::vector<short>>& channels_data )
{
    printf( ">>>> extract_data_int16( %s )\n", filename );
    wav_errors_e err;
    wav_header_s header;
    err = read_header( filename, &header );
    if ( err != WAV_OK ) {
        // Problems with reading a header.
        return err;
    }

    if ( header.bitsPerSample != 16 ) {
        // Only 16-bit samples is supported.
        return UNSUPPORTED_FORMAT;
    }

    FILE* f = fopen( filename, "rb" );
    if ( !f ) {
        return IO_ERROR;
    }
    fseek( f, 44, SEEK_SET ); // Seek to the begining of PCM data.

    int chan_count = header.numChannels;
    int samples_per_chan = ( header.subchunk2Size / sizeof(short) ) / chan_count;

    // 1. Reading all PCM data from file to a single vector.
    std::vector<short> all_channels;
    all_channels.resize( chan_count * samples_per_chan );
    size_t read_bytes = fread( all_channels.data(), 1, header.subchunk2Size, f );
    if ( read_bytes != header.subchunk2Size ) {
        printf( "extract_data_int16() read only %zu of %u\n", read_bytes, header.subchunk2Size );
        return IO_ERROR;
    }
    fclose( f );


    // 2. Put all channels to its own vector.
    channels_data.resize( chan_count );
    for ( size_t ch = 0; ch < channels_data.size(); ch++ ) {
        channels_data[ ch ].resize( samples_per_chan );
    }

    for ( int ch = 0; ch < chan_count; ch++ ) {
        std::vector<short>& chdata = channels_data[ ch ];
        for ( size_t i = 0; i < samples_per_chan; i++ ) {
            chdata[ i ] = all_channels[ chan_count * i + ch ];
        }
    }
    return WAV_OK;
}


wav_headers_errors_e check_header( const wav_header_s *header_ptr, size_t file_size_bytes )
{
    // Go to wav_header.h for details

    if ( header_ptr->chunkId[0] != 0x52 ||
         header_ptr->chunkId[1] != 0x49 ||
         header_ptr->chunkId[2] != 0x46 ||
         header_ptr->chunkId[3] != 0x46 )
    {
        printf( "HEADER_RIFF_ERROR\n" );
        return HEADER_RIFF_ERROR;
    }

    if ( header_ptr->chunkSize != file_size_bytes - 8 ) {
        printf( "HEADER_FILE_SIZE_ERROR\n" );
        return HEADER_FILE_SIZE_ERROR;
    }

    if ( header_ptr->format[0] != 0x57 ||
         header_ptr->format[1] != 0x41 ||
         header_ptr->format[2] != 0x56 ||
         header_ptr->format[3] != 0x45 )
    {
        printf( "HEADER_WAVE_ERROR\n" );
        return HEADER_WAVE_ERROR;
    }

    if ( header_ptr->subchunk1Id[0] != 0x66 ||
         header_ptr->subchunk1Id[1] != 0x6d ||
         header_ptr->subchunk1Id[2] != 0x74 ||
         header_ptr->subchunk1Id[3] != 0x20 )
    {
        printf( "HEADER_FMT_ERROR\n" );
        return HEADER_FMT_ERROR;
    }

    if ( header_ptr->audioFormat != 1 ) {
        printf( "HEADER_NOT_PCM\n" );
        return HEADER_NOT_PCM;
    }

    if ( header_ptr->subchunk1Size != 16 ) {
        printf( "HEADER_SUBCHUNK1_ERROR\n" );
        return HEADER_SUBCHUNK1_ERROR;
    }

    if ( header_ptr->byteRate != header_ptr->sampleRate * header_ptr->numChannels * header_ptr->bitsPerSample/8 ) {
        printf( "HEADER_BYTES_RATE_ERROR\n" );
        return HEADER_BYTES_RATE_ERROR;
    }

    if ( header_ptr->blockAlign != header_ptr->numChannels * header_ptr->bitsPerSample/8 ) {
        printf( "HEADER_BLOCK_ALIGN_ERROR\n" );
        return HEADER_BLOCK_ALIGN_ERROR;
    }

    if ( header_ptr->subchunk2Id[0] != 0x64 ||
         header_ptr->subchunk2Id[1] != 0x61 ||
         header_ptr->subchunk2Id[2] != 0x74 ||
         header_ptr->subchunk2Id[3] != 0x61 )
    {
        printf( "HEADER_FMT_ERROR\n" );
        return HEADER_FMT_ERROR;
    }

    if ( header_ptr->subchunk2Size != file_size_bytes - 44 )
    {
        printf( "HEADER_SUBCHUNK2_SIZE_ERROR\n" );
        return HEADER_SUBCHUNK2_SIZE_ERROR;
    }

    return HEADER_OK;
}

void prefill_header(wav_header_s *header_ptr)
{
    // Go to wav_header.h for details

    header_ptr->chunkId[0] = 0x52;
    header_ptr->chunkId[1] = 0x49;
    header_ptr->chunkId[2] = 0x46;
    header_ptr->chunkId[3] = 0x46;

    header_ptr->format[0] = 0x57;
    header_ptr->format[1] = 0x41;
    header_ptr->format[2] = 0x56;
    header_ptr->format[3] = 0x45;

    header_ptr->subchunk1Id[0] = 0x66;
    header_ptr->subchunk1Id[1] = 0x6d;
    header_ptr->subchunk1Id[2] = 0x74;
    header_ptr->subchunk1Id[3] = 0x20;

    header_ptr->subchunk2Id[0] = 0x64;
    header_ptr->subchunk2Id[1] = 0x61;
    header_ptr->subchunk2Id[2] = 0x74;
    header_ptr->subchunk2Id[3] = 0x61;

    header_ptr->audioFormat   = 1;
    header_ptr->subchunk1Size = 16;
    header_ptr->bitsPerSample = 16;

}

wav_errors_e fill_header(wav_header_s *header_ptr, int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan)
{
    if ( bits_per_sample != 16 ) {
        return UNSUPPORTED_FORMAT;
    }

    if ( chan_count < 1 ) {
        return BAD_PARAMS;
    }
    prefill_header( header_ptr );

    int file_size_bytes = 44 + chan_count * (bits_per_sample/8) * samples_count_per_chan;

    header_ptr->sampleRate    = sample_rate;
    header_ptr->numChannels   = chan_count;
    header_ptr->bitsPerSample = 16;

    header_ptr->chunkSize     = file_size_bytes - 8;
    header_ptr->subchunk2Size = file_size_bytes - 44;

    header_ptr->byteRate      = header_ptr->sampleRate * header_ptr->numChannels * header_ptr->bitsPerSample/8;
    header_ptr->blockAlign    = header_ptr->numChannels * header_ptr->bitsPerSample/8;

    return WAV_OK;
}

wav_errors_e make_wav_file(const char* filename, int sample_rate, const std::vector< std::vector<short> > &channels_data)
{
    printf( ">>>> make_wav_file( %s )\n", filename );
    wav_errors_e err;
    wav_header_s header;

    int chan_count = (int)channels_data.size();

    if ( chan_count < 1 ) {
        return BAD_PARAMS;
    }

    int samples_count_per_chan = (int)channels_data[0].size();

    // Verify that all channels have the same number of samples.
    for ( size_t ch = 0; ch < chan_count; ch++ ) {
        if ( channels_data[ ch ].size() != (size_t) samples_count_per_chan ) {
            return BAD_PARAMS;
        }
    }

    err = fill_header( &header, chan_count, 16, sample_rate, samples_count_per_chan );
    if ( err != WAV_OK ) {
        return err;
    }

    std::vector<short> all_channels;
    all_channels.resize( chan_count * samples_count_per_chan );

    for ( int ch = 0; ch < chan_count; ch++ ) {
        const std::vector<short>& chdata = channels_data[ ch ];
        for ( size_t i = 0; i < samples_count_per_chan; i++ ) {
            all_channels[ chan_count * i + ch ] = chdata[ i ];
        }
    }

    FILE* f = fopen( filename, "wb" );
    fwrite( &header, sizeof(wav_header_s), 1, f );
    fwrite( all_channels.data(), sizeof(short), all_channels.size(), f );
    if ( !f ) {
        return IO_ERROR;
    }

    fclose( f );

    return WAV_OK;
}

void null_header(wav_header_s *header_ptr)
{
    memset( header_ptr, 0, sizeof(wav_header_s) );
}

wav_errors_e make_mono(const std::vector<std::vector<short> > &source, std::vector< std::vector<short> > &dest_mono)
{
    int chan_count = (int)source.size();

    if ( chan_count != 2 ) {
        return BAD_PARAMS;
    }

    int samples_count_per_chan = (int)source[0].size();

    // Verify that all channels have the same number of samples.
    for ( size_t ch = 0; ch < chan_count; ch++ ) {
        if ( source[ ch ].size() != (size_t) samples_count_per_chan ) {
            return BAD_PARAMS;
        }
    }

    dest_mono.resize( 1 );
    std::vector<short>& mono = dest_mono[ 0 ];
    mono.resize( samples_count_per_chan );

    // Mono channel is an arithmetic mean of all (two) channels.
    for ( size_t i = 0; i < samples_count_per_chan; i++ ) {
        mono[ i ] = ( source[0][i] + source[1][i] ) / 2;
    }

    return WAV_OK;
}

wav_errors_e make_reverb(std::vector<std::vector<short> > &sounds, int sample_rate, double delay_seconds, float decay)
{
    int chan_count = (int)sounds.size();

    if ( chan_count < 1 ) {
        return BAD_PARAMS;
    }

    int samples_count_per_chan = (int)sounds[0].size();

    // Verify that all channels have the same number of samples.
    for ( size_t ch = 0; ch < chan_count; ch++ ) {
        if ( sounds[ ch ].size() != (size_t) samples_count_per_chan ) {
            return BAD_PARAMS;
        }
    }

    int delay_samples = (int)(delay_seconds * sample_rate);


    for ( size_t ch = 0; ch < chan_count; ch++ ) {
        std::vector<float> tmp;
        tmp.resize(sounds[ch].size());

        // Convert signal from short to float
        for ( size_t i = 0; i < samples_count_per_chan; i++ ) {
            tmp[ i ] = sounds[ ch ][ i ];
        }

        // Add a reverb
        for ( size_t i = 0; i < samples_count_per_chan - delay_samples; i++ ) {
            tmp[ i + delay_samples ] += decay * tmp[ i ];
        }

        // Find maximum signal's magnitude
        float max_magnitude = 0.0f;
        for ( size_t i = 0; i < samples_count_per_chan - delay_samples; i++ ) {
            if ( abs(tmp[ i ]) > max_magnitude ) {
                max_magnitude = abs(tmp[ i ]);
            }
        }

        // Signed short can keep values from -32768 to +32767,
        // After reverb, usually there are values large 32000.
        // So we must scale all values back to [ -32768 ... 32768 ]
        float norm_coef = 30000.0f / max_magnitude;
        printf( "max_magnitude = %.1f, coef = %.3f\n", max_magnitude, norm_coef );

        // Scale back and transform floats to shorts.
        for ( size_t i = 0; i < samples_count_per_chan; i++ ) {
            sounds[ ch ][ i ] = (short)(norm_coef * tmp[ i ]);
        }
    }

    return WAV_OK;
}
