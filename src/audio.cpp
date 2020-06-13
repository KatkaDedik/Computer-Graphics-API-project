#include "audio.h"

#include <stdexcept>

#define DR_MP3_IMPLEMENTATION
#include <dr_mp3.h>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

#include <vector>


static void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {
  ma_decoder *pDecoder = (ma_decoder *)pDevice->pUserData;
  if (pDecoder == NULL) {
    return;
  }

  ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

  std::vector<short> smth;

  for (int i = 0; i < 20; i++) {
    smth.emplace_back(((short*)pOutput)[i]);
  }

  (void)pInput;
}

Audio::Audio(const std::string &filename) {
  
  ma_result result = ma_decoder_init_file(filename.c_str(), NULL, &decoder);
  if (result != MA_SUCCESS) {
    throw std::logic_error("wtf!");
  }

  

  ma_device_config deviceConfig;

  
  deviceConfig = ma_device_config_init(ma_device_type_playback);	
  deviceConfig.playback.format = decoder.outputFormat;
  deviceConfig.playback.channels = decoder.outputChannels;
  deviceConfig.sampleRate = decoder.outputSampleRate;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = &decoder;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
    ma_decoder_uninit(&decoder);
    throw std::logic_error("Failed to open playback device.");
  }

  if (ma_device_start(&device) != MA_SUCCESS) {
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    throw std::logic_error("Failed to start playback device.");
  }

}

Audio::~Audio() {
  ma_device_uninit(&device);
  ma_decoder_uninit(&decoder);
}