#include "hzpch.h"
#include "Audio.h"


namespace Hazel
{
    bool AudioPlayer::LoadAudioFile(const std::string& filePath) {
        if (decoderInitialized) {
            ma_decoder_uninit(&decoder);  // Unload the current file if one is loaded
        }

        if (ma_decoder_init_file(filePath.c_str(), NULL, &decoder) != MA_SUCCESS) {
            HZ_ERROR("Failed to load audio file: {0}", filePath);
            return false;
        }

        decoderInitialized = true;
        return true;
    }

    void AudioPlayer::Play() {
        if (decoderInitialized && !isPlaying) {
            ma_device_start(&device);
            isPlaying = true;
        }
    }

    void AudioPlayer::Pause() {
        if (isPlaying) {
            ma_device_stop(&device);
            isPlaying = false;
        }
    }

    void AudioPlayer::Replay() {
        if (decoderInitialized) {
           
            ma_decoder_seek_to_pcm_frame(&decoder, 0);
            Play();
        }
    }

    AudioPlayer::AudioPlayer() {
        ma_device_config config = ma_device_config_init(ma_device_type_playback);
        config.playback.format = ma_format_f32;
        config.playback.channels = 2;
        config.sampleRate = 44100;
        config.dataCallback = AudioCallback;
        config.pUserData = this;

        if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
            HZ_ERROR("Failed to initialize audio device.");
        }
    }

    AudioPlayer::~AudioPlayer() {
        ma_device_uninit(&device);
        if (decoderInitialized) {
            ma_decoder_uninit(&decoder);
        }
    }

    void AudioPlayer::AudioCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
        AudioPlayer* pAudioPlayer = static_cast<AudioPlayer*>(pDevice->pUserData);
        if (pAudioPlayer->decoderInitialized) {
            ma_decoder_read_pcm_frames(&pAudioPlayer->decoder, pOutput, frameCount, NULL);
        }
        else {
            std::fill_n(static_cast<float*>(pOutput), frameCount * 2, 0.0f); // Fill with silence
        }
    }

    bool AudioPlayer::playing()
    {
        return isPlaying;
    }

}
