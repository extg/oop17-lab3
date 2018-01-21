#ifndef LAB3_WAVDATA_H
#define LAB3_WAVDATA_H


#include <string>
#include <vector>
#include "wav_header.h"
#include "WavError.h"

class WavData {
private:
  static const int HEADER_SIZE = 44;

  std::string filename;
  wav_header_s header;
  std::vector< std::vector<short> > channels_data; // PCM data

  void ReadHeader() throw(WavError);
  void CheckHeader(size_t file_size_bytes) throw(WavError);
  void PrefillHeader();
  void FillHeader(int chan_count, int bits_per_sample, int sample_rate, int samples_count_per_chan) throw(WavError);
  void ExtractDataInt16() throw(WavError);
  void NullHeader();

public:
  WavData();
  void CreateFromFile(const std::string &f);
  std::string GetDescription();
  void ConvertStereoToMono() throw(WavError);
  void ApplyReverb(double delay_seconds, float decay) throw(WavError);
  void SaveToFile(const char* filename) throw(WavError);
};


#endif //LAB3_WAVDATA_H
