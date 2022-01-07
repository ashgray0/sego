#pragma once
#ifndef CSO_HARDWARE_BSP_H
#define CSO_HARDWARE_BSP_H

#include "daisy_seed.h"

namespace daisy
{
class CsoHardware
{
  public:

    enum Sw
    {
        SW_1,
        SW_2,
        SW_3,
        SW_4,
        SW_LAST,
    };

    enum Knob
    {
        KNOB_1,
        KNOB_2,
        KNOB_3,
        KNOB_4,
        KNOB_5,
        KNOB_6,
        KNOB_7,
        KNOB_8,
        KNOB_9,
        KNOB_10,
        KNOB_11,
        KNOB_12,
        KNOB_LAST,
    };

    CsoHardware() {}
    ~CsoHardware() {}

    void Init(bool boost = false);

    void DelayMs(size_t del);

    void StartAudio(AudioHandle::InterleavingAudioCallback cb);

    void StartAudio(AudioHandle::AudioCallback cb);

    void ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb);

    void ChangeAudioCallback(AudioHandle::AudioCallback cb);

    void StopAudio();

    void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);

    float AudioSampleRate();

    void SetAudioBlockSize(size_t blocksize);

    size_t AudioBlockSize();

    float AudioCallbackRate();

    void StartAdc();

    void StopAdc();

    void ProcessAnalogControls();

    inline void ProcessAllControls()
    {
        ProcessAnalogControls();
        ProcessDigitalControls();
    }

    float GetKnobValue(Knob k);

    void ProcessDigitalControls();

    void ClearLeds();

    void UpdateLeds();

    DaisySeed     seed;
    AnalogControl knob1,
        knob2,
        knob3,
        knob4,
        knob5,
        knob6,
        knob7,
        knob8,
        knob9,
        knob10,
        knob11,
        knob12,
        *knobs[KNOB_LAST];
    Switch sw1,
        sw2,
        sw3,
        sw4,
        *switches[SW_LAST];
    RgbLed led1,
        led2;
    MidiUartHandler midi;

  private:
    void SetHidUpdateRates();
    void InitSwitches();
    void InitLeds();
    void InitKnobs();
    void InitMidi();
};

}
#endif