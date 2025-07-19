#pragma once

#include <OpenAL/OpenAL.h>
#include <dr_wav/dr_wav.h>
#include <string>
#include <memory>
#include <vector>
#include <map>

class Source {
    public:
        Source(const std::string& wavsrc);
        ~Source();

        void play();
        void stop();

    private:
        ALuint buffer, source;
        int16_t* data;

        unsigned int channels;
        unsigned int sampleRate;
        drwav_uint64 frameCount;
};

class Audio {
    public:
        Audio(std::vector<std::string> soundsrcs);
        ~Audio();

        void play(const std::string& track);
        void stop(const std::string& track);

    private:
        ALCdevice* device;
        ALCcontext* context;

        std::map<std::string, std::unique_ptr<Source>> sources;
};