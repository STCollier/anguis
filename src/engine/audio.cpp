#define DR_WAV_IMPLEMENTATION

#include "audio.hpp"
#include <iostream>
#include <format>

Source::Source(const std::string& wavsrc) {
    data = drwav_open_file_and_read_pcm_frames_s16(wavsrc.c_str(), &channels, &sampleRate, &frameCount, nullptr);
    if (!data) {
        throw std::runtime_error(std::format("Failed to load WAV file: {}", wavsrc));
    }
    ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, data, frameCount * channels * sizeof(int16_t), sampleRate);

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
}

void Source::play() {
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
        alSourceStop(source);
        alSourceRewind(source);
        alSourcePlay(source);
    }
}

void Source::stop() {
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state != AL_STOPPED) {
        alSourceStop(source);
    }
}

Source::~Source() {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    drwav_free(data, nullptr);
}

Audio::Audio(std::vector<std::string> soundsrcs) {
    device = alcOpenDevice(nullptr); // Default device
    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);

    for (std::string& src : soundsrcs) {
        sources[src] = std::make_unique<Source>(src);
    }
}

Audio::~Audio() {
    /*for (const auto& [str, ptr] : sources) {
        ptr.reset();
    }*/

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void Audio::play(const std::string& track) {
    auto& source = sources[track];
    if (source) {
        source->play();
    }
}

void Audio::stop(const std::string& track) {
    auto& source = sources[track];
    if (source) {
        source->stop();
    }
}