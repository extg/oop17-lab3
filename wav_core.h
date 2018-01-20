#ifndef WAV_CORE_H
#define WAV_CORE_H

#include <vector>

#include "wav_header.h"


// TODO: Implement all this in the form of a class.
// TODO: Use an exception system to control errors.
// TODO: Make support for 8-bit PSM, not just 16-bit.
// TODO: Write a function to change the tone of the voice. (Interestingly, it's not too difficult?)


// *********************************************************************
// * Error handling
// *********************************************************************

// Possible errors
enum wav_errors_e {
    WAV_OK = 0,
    IO_ERROR,
    BAD_FORMAT,
    UNSUPPORTED_FORMAT,
    BAD_PARAMS,
    DATA_SIZE_ERROR
};

// Possible header's errors
enum wav_headers_errors_e {
    HEADER_OK = 0,
    HEADER_RIFF_ERROR,
    HEADER_FILE_SIZE_ERROR,
    HEADER_WAVE_ERROR,
    HEADER_FMT_ERROR,
    HEADER_NOT_PCM,
    HEADER_SUBCHUNK1_ERROR,
    HEADER_BYTES_RATE_ERROR,
    HEADER_BLOCK_ALIGN_ERROR,
    HEADER_SUBCHUNK2_SIZE_ERROR
};


// ************************************************************************
// * Functions for working with WAV file
// ************************************************************************

// Reads file 'filename' and puts header's data to 'header_ptr' address.
// Also checks header validity, returns 'WAV_OK' on success.
wav_errors_e read_header( const char* filename, wav_header_s* header_ptr );

// Prints header's data from 'header_ptr' address.
void print_info( const wav_header_s* header_ptr );

// Reads file 'filename' and puts PCM data (raw sound data) to 'channels_data'.
// Also checks header validity, returns 'WAV_OK' on success.
wav_errors_e extract_data_int16( const char* filename, std::vector<std::vector<short>>& channels_data );


// Creates a new WAV file 'filename', using 'sample_rate' and PCM data from 'channels_data'.
// Returns 'WAV_OK' on success.
wav_errors_e make_wav_file( const char* filename, int sample_rate, const std::vector<std::vector<short>>& channels_data );


// ************************************************************************
// * Functions for working with sound PCM data - Digital Signal Processing
// ************************************************************************

// Makes mono PCM data from stereo 'source'.
// Returns 'WAV_OK' on success.
wav_errors_e make_mono( const std::vector<std::vector<short>>& source, std::vector<std::vector<short>>& dest_mono );


// ************************************************************************
// * Private functions
// ************************************************************************

// Fills header with zeroes.
void null_header( wav_header_s* header_ptr );

// Checks header validity.
// Returns 'WAV_OK' on success.
wav_headers_errors_e check_header( const wav_header_s* header_ptr, size_t file_size_bytes );

// Fills header information, using input parameters. This function calls prefill_header() itself.
wav_errors_e fill_header( wav_header_s* header_ptr, int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan );

// Fills 'header_ptr' with default values.
void prefill_header( wav_header_s* header_ptr );


#endif // WAV_CORE_H
