#pragma once
#include "miniaudio.h"

#include <string>
#include "Hazel/Core/Core.h"

namespace Hazel {

    class AudioPlayer {
    public:
        static AudioPlayer& GetInstance() {
            static AudioPlayer instance;
            return instance;
        }

        AudioPlayer(AudioPlayer const&) = delete;
        void operator=(AudioPlayer const&) = delete;

        bool LoadAudioFile(const std::string& filePath);

        void Play();
        void Pause();
        void Replay();

        bool playing();

    private:
        AudioPlayer();

        ~AudioPlayer();

        static void AudioCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

        ma_device device;
        ma_decoder decoder;
        bool decoderInitialized = false;
        bool isPlaying = false;
    };

}
