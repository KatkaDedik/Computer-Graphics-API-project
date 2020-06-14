#pragma once

#include <miniaudio.h>
#include <string>

class Audio {

  ma_decoder decoder;
  ma_device device;
  bool is_initialized = false;

public:
  Audio(const std::string &filename);
  Audio() = default;
  ~Audio();

  //Audio operator=(const Audio &a) : decoder(a.decoder), device(a.device) {}
};
