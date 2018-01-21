#include <iostream>

#include "WavData.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "************** | WavCore | **************" << endl;

    try {
        WavData wav1;
        wav1.CreateFromFile( "../0.wav" );
        cout << wav1.GetDescription() << endl;
        wav1.ConvertStereoToMono();
        wav1.ApplyReverb( 0.500, 0.6f );
        wav1.SaveToFile( "../0-mono.wav" );
    } catch ( WavError& we ) {
        cerr << we.what() << endl;
    }
    return 0;
}
