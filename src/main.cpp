#include <iostream>

#include "wav_core.h"
#include "WavData.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "************** | WavCore | **************" << endl;

    WavData wav1;
    wav1.CreateFromFile( "../0.wav" );
    cout << wav1.GetDescription() << endl;
    wav1.ConvertStereoToMono();
    wav1.ApplyReverb( 0.500, 0.6f );
    wav1.SaveToFile( "../0-mono.wav" );

//    try {
//        WavData wav1;
//        wav1.CreateFromFile( "../0.wav" );
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
