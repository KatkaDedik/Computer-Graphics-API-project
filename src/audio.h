#pragma once

#include <miniaudio.h>
#include <string>


class Audio
{

  ma_decoder decoder;
  ma_device device;

public:
  Audio(const std::string &filename);
  ~Audio();

};
