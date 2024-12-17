#pragma once

#include "ZC_StreamSound.h"
#include "ZC/Tools/ZC_uptr.h"

class ZC_Sound;
// //  Unique pointer to ZC_Sound.
// using ZC_upSound = ZC_uptr<ZC_Sound>;

//  Class for controlling sound in an audio stream.
class ZC_Sound
{
public:
    ZC_Sound(const ZC_sptr<ZC_StreamSound>& _spStream_sound);

    ZC_Sound(ZC_Sound&&);
    ZC_Sound& operator = (ZC_Sound&&);

    ~ZC_Sound();
    
    //  Begins transferring audio data to an existing audio stream.
    void Play();

    //  Begins or continues to transmit audio data in an infinitely repeating state to an existing audio stream/
    void PlayLoop();

    //  Stops transmission of data to the audio stream, maintaining the current transmission position.
    void Pause();

    //  Stops data transmission to the audio stream, returning the gear position to the start.
    void Stop();

    /*
    Sounds state.

    Return:
    Current sound state.
    */
    ZC_SoundState GetState();

    /*
    Set volume of the sound.

    Params:
    _volume - the volume value is in the range 0–100, otherwise it will be adjusted to the nearest limit
    */
    void SetVolume(float _volume) noexcept;

    /*
    Helps to get volume of the sound.

    Return:
    Current volume in range 0 - 100.
    */
    short GetVolume() const noexcept;

    /*
    Helps to get copy of current sound with default start values (volume - 100, playing state - stop).

    Return:
    Unique pointer with the same sound data.
    */
    ZC_Sound GetSameSound() const;

    //  Return audioset of the sound if sound exists, otherwise nullptr.
    const ZC_AudioSet* GetAudioSet() const noexcept;

private:
    ZC_sptr<ZC_StreamSound> spStream_sound;
    ZC_SConnection sconGetpZC_StreamSound;

    ZC_sptr<ZC_StreamSound> GetpZC_StreamSound();
};