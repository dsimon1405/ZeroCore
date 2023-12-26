#include "ZC_oboe_AudioStream.h"

#include <ZC/ErrorLogger/ZC_ErrorLogger.h>

ZC_oboe_AudioStream::ZC_oboe_AudioStream(const ZC_AudioSet& _audioSet)
    : ZC_AudioStream(_audioSet)
{
    if (_audioSet.frequency <= 0)
    {
        ZC_ErrorLogger::Err("_audioSet.frequency must be > 0!", __FILE__, __LINE__);
        return;
    }

    InitAudioStream();
}

ZC_oboe_AudioStream::~ZC_oboe_AudioStream()
{
    spAudioStream->close();
}

void ZC_oboe_AudioStream::InitAudioStream()
{
    oboe::AudioStreamBuilder builder;
    builder.setFormatConversionAllowed(true);
    builder.setPerformanceMode(oboe::PerformanceMode::LowLatency);
    builder.setSharingMode(oboe::SharingMode::Exclusive);
    builder.setSampleRateConversionQuality(oboe::SampleRateConversionQuality::Medium);

    switch (audioSet.bitsPerSample)
    {
        case ZC_AudioSet::BitsPerSample::S16:
            builder.setFormat(oboe::AudioFormat::I16);
            break;
        case ZC_AudioSet::BitsPerSample::S32:
            builder.setFormat(oboe::AudioFormat::I32);
            break;
    }
    builder.setSampleRate(audioSet.frequency);
    builder.setChannelCount(audioSet.channels);

    builder.setDataCallback(this);
    builder.setErrorCallback(this);

    oboe::Result result = builder.openStream(spAudioStream);
    if (result != oboe::Result::OK)
    {
        ZC_ErrorLogger::Err("oboe openStream() fail: " + std::string(oboe::convertToText(result)), __FILE__, __LINE__);
        return;
    }

    result = spAudioStream->requestStart();
    if (result != oboe::Result::OK)
    {
        ZC_ErrorLogger::Err("oboe requestStart() fail: " + std::string(oboe::convertToText(result)), __FILE__, __LINE__);
        return;
    }
    
    ZC_ErrorLogger::Clear();
}

oboe::DataCallbackResult ZC_oboe_AudioStream::onAudioReady(oboe::AudioStream *audioStream, void* audioData, int32_t numFrames)
{
    static int frameLength = static_cast<int>(audioSet.channels) * static_cast<int>(audioSet.bitsPerSample) / 8;    //  8 - bits in byte
    GetStreamData(audioData, numFrames * frameLength);
    return oboe::DataCallbackResult::Continue;
//    return oboe::DataCallbackResult::Stop;
}

void ZC_oboe_AudioStream::onErrorAfterClose(oboe::AudioStream* audioStream, oboe::Result result)
{
    if (result == oboe::Result::ErrorDisconnected)
    {
        InitAudioStream();
    }
}
