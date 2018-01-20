#include <iostream>

#include "wav_core.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "************** | WavCore | **************" << endl;


    // ################  Tests for WavCore  ################

    const char* input_fname  = "../0.wav";
    const char* output_fname = "../0.out.wav";

    wav_errors_e err;
    wav_header_s header;


    // #### Opening WAV file, checking header.
    err = read_header( input_fname, &header );
    if ( err != WAV_OK ) {
        cerr << "read_header() error: " << (int)err << endl;
        print_info( &header );
        return err;
    }


    // #### Printing header.
    print_info( &header );


    // #### Reading PCM data from file.
    vector< vector<short> > chans_data;
    err = extract_data_int16( input_fname, chans_data );
    if ( err != WAV_OK ) {
        cerr << "extract_data_int16() error: " << (int)err << endl;
        return err;
    }
    cout << endl << "********************" << endl;


    // #### Make several changes to PCM data.

    // # Making signal mono from stereo.
    vector< vector<short> > edited_data;
    err = make_mono( chans_data, edited_data );
    if ( err != WAV_OK ) {
        cerr << "make_mono() error: " << (int)err << endl;
        return err;
    }

    // # Add a reverberation
    make_reverb(edited_data, header.sampleRate, 0.5, 0.6f);


    // #### Making new WAV file using edited PCM data.
    err = make_wav_file( output_fname, header.sampleRate, edited_data );
    if ( err != WAV_OK ) {
        cerr << "make_wav_file() error: " << (int)err << endl;
        print_info( &header );
        return err;
    }


    // #### Reading the file just created to check its header corectness.
    err = read_header( output_fname, &header );
    if ( err != WAV_OK ) {
        cerr << "read_header() error: " << (int)err << endl;
        print_info( &header );
        return err;
    }
    print_info( &header );



//    try {
//        WavData wav1;
//        wav1.CreateFromFile( "chornye-glaza.wav" );
//        cout << wav1.GetDescription() << endl;
//        wav1.ConvertStereoToMono();
//        wav1.ApplyReverb( 0.500, 0.6f );
//        wav1.SaveToFile( "black-eyes-mono.wav" );
//
//        WavData wav2( "secret-speech.wav" );
//        cout << wav2.GetDescription() << endl;
//        int sampleRate << wav2.GetSampleRate();
//        int chanCount = wav2.GetChanCount();
//        bool isStereo = wav2.IsStereo();
//
//        wav2.ChangeSampleRate( 22050 );
//
//        // Cut first 10 seconds and last 20 seconds
//        wav2.CutBegin( 10.0 );
//        wav2.CutEnd( 20.0 );
//
//        wav2.Save(); // Overwrite the original file.
//
//    } catch ( WavError& we ) {
//        cerr << we.what();
//    }
    return 0;
}
