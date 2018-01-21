#ifndef LAB3_WAVDATA_H
#define LAB3_WAVDATA_H


#include <string>
#include "wav_core.h"
#include "wav_header.h"

class WavData {
private:
  static const int HEADER_SIZE = 44;

  std::string filename;
  wav_header_s header;
  std::vector< std::vector<short> > channels_data; // PCM data

  wav_errors_e ReadHeader();
  wav_headers_errors_e CheckHeader(size_t file_size_bytes);
  wav_errors_e ExtractDataInt16();
  void NullHeader();

public:
  WavData();
  void CreateFromFile(const std::string &f);
  std::string GetDescription();
  wav_errors_e ConvertStereoToMono();
  wav_errors_e ApplyReverb(double delay_seconds, float decay);
  wav_errors_e SaveToFile(const char* filename);
};


#endif //LAB3_WAVDATA_H
