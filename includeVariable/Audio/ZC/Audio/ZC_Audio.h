#pragma once

#include "ZC_AudioStream.h"
#include "ZC/Tools/ZC_uptr.h"

/*
Class for creating an audio stream.
*/
class ZC_Audio
{
public:
    ZC_Audio() = delete;

    /*
    Create ZC_AudioStream object. Be sure to call Close Audio Stream when the audio stream is no longer needed to correctly release resources

    Params:
    _audioSet - a set of audio parameters for an audio stream.

    Return:
    On success true, else false (ZC_ErrorLogger::ErrorMessage() - for more information).
    */
    static bool OpenAudioStream(const ZC_AudioSet& _audioSet);

    //  Closes the audio stream if it was open.
    static void CloseAudioStream() noexcept;

    /*
    Reopens an audio stream (using the last opened ZC_AudioSet) if it was previously opened and closed.

    Return:
    If success - true, otherwise false.
    */
    static bool ReopenAudioStream();

private:
    static inline ZC_uptr<ZC_AudioStream> upAudioStream;
};