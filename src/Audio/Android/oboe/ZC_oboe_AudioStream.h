#pragma once

#include <ZC/Audio/ZC_AudioStream.h>

#include <oboe/AudioStream.h>
#include <oboe/AudioStreamCallback.h>

class ZC_oboe_AudioStream : public ZC_AudioStream, public oboe::AudioStreamDataCallback, public oboe::AudioStreamErrorCallback
{
public:
    ZC_oboe_AudioStream(const ZC_AudioSet& _audioSet);
    ~ZC_oboe_AudioStream() override;

private:
    std::shared_ptr<oboe::AudioStream> spAudioStream;

    void InitAudioStream();
    //  oboe::AudioStreamDataCallback::onAudioReady(...)
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override;
    //  oboe::AudioStreamErrorCallback::onErrorAfterClose(...)
    void onErrorAfterClose(oboe::AudioStream* audioStream, oboe::Result result) override;
};