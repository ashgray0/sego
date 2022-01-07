#include "cso_hardware.h"

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 96028.f
#endif

#define PIN_SW_1 10
#define PIN_SW_2 9
#define PIN_SW_3 8
#define PIN_SW_4 7

#define PIN_KNOB_1 15
#define PIN_KNOB_2 16
#define PIN_KNOB_3 17
#define PIN_KNOB_4 18
#define PIN_KNOB_5 19
#define PIN_KNOB_6 20

#define PIN_KNOB_7  21
#define PIN_KNOB_8  22
#define PIN_KNOB_9  23
#define PIN_KNOB_10 24
#define PIN_KNOB_11 25
#define PIN_KNOB_12 28


using namespace daisy;  

void CsoHardware::Init(bool boost)
{
    seed.Configure();
    seed.Init(boost);
    InitSwitches();
    InitLeds();
    InitKnobs();
    InitMidi();
    SetAudioBlockSize(48);
}

void CsoHardware::DelayMs(size_t del)
{
    seed.DelayMs(del);
}


void CsoHardware::SetHidUpdateRates()
{
    for(int i = 0; i < KNOB_LAST; i++)
    {
        knobs[i]->SetSampleRate(AudioCallbackRate());
    }
}

void CsoHardware::StartAudio(AudioHandle::InterleavingAudioCallback cb)
{
    seed.StartAudio(cb);
}

void CsoHardware::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void CsoHardware::ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void CsoHardware::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void CsoHardware::StopAudio()
{
    seed.StopAudio();
}

void CsoHardware::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
    SetHidUpdateRates();
}

size_t CsoHardware::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

void CsoHardware::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
    SetHidUpdateRates();
}

float CsoHardware::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

float CsoHardware::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void CsoHardware::StartAdc()
{
    seed.adc.Start();
}

void CsoHardware::StopAdc()
{
    seed.adc.Stop();
}

void CsoHardware::ProcessAnalogControls()
{
    knob1.Process();
    knob2.Process();
    knob3.Process();
    knob4.Process();
    knob5.Process();
    knob6.Process();

    knob7.Process();
    knob8.Process();
    knob9.Process();
    knob10.Process();
    knob11.Process();
    knob12.Process();
}

float CsoHardware::GetKnobValue(Knob k)
{
    size_t idx;
    idx = k < KNOB_LAST ? k : KNOB_1;
    return knobs[idx]->Value();
}

void CsoHardware::ProcessDigitalControls()
{
    sw1.Debounce();
    sw2.Debounce();
    sw3.Debounce();
    sw4.Debounce();
}

void CsoHardware::ClearLeds()
{
    Color c;
    c.Init(Color::PresetColor::OFF);
    led1.SetColor(c);
    led2.SetColor(c);
}

void CsoHardware::UpdateLeds()
{
    led1.Update();
    led2.Update();
}

void CsoHardware::InitSwitches()
{
    sw1.Init(seed.GetPin(PIN_SW_1));
    sw2.Init(seed.GetPin(PIN_SW_2));
    sw3.Init(seed.GetPin(PIN_SW_3));
    sw4.Init(seed.GetPin(PIN_SW_4));

    switches[SW_1] = &sw1;
    switches[SW_2] = &sw2;
    switches[SW_3] = &sw3;
    switches[SW_4] = &sw4;
}

void CsoHardware::InitLeds()
{
    ClearLeds();
    UpdateLeds();
}
void CsoHardware::InitKnobs()
{
    AdcChannelConfig knob_init[KNOB_LAST];
    knob_init[KNOB_1].InitSingle(seed.GetPin(PIN_KNOB_1));
    knob_init[KNOB_2].InitSingle(seed.GetPin(PIN_KNOB_2));
    knob_init[KNOB_3].InitSingle(seed.GetPin(PIN_KNOB_3));
    knob_init[KNOB_4].InitSingle(seed.GetPin(PIN_KNOB_4));
    knob_init[KNOB_5].InitSingle(seed.GetPin(PIN_KNOB_5));
    knob_init[KNOB_6].InitSingle(seed.GetPin(PIN_KNOB_6));

    knob_init[KNOB_7].InitSingle(seed.GetPin(PIN_KNOB_7));
    knob_init[KNOB_8].InitSingle(seed.GetPin(PIN_KNOB_8));
    knob_init[KNOB_9].InitSingle(seed.GetPin(PIN_KNOB_9));
    knob_init[KNOB_10].InitSingle(seed.GetPin(PIN_KNOB_10));
    knob_init[KNOB_11].InitSingle(seed.GetPin(PIN_KNOB_11));
    knob_init[KNOB_12].InitSingle(seed.GetPin(PIN_KNOB_12));

    seed.adc.Init(knob_init, KNOB_LAST);

    knobs[KNOB_1]  = &knob1;
    knobs[KNOB_2]  = &knob2;
    knobs[KNOB_3]  = &knob3;
    knobs[KNOB_4]  = &knob4;
    knobs[KNOB_5]  = &knob5;
    knobs[KNOB_6]  = &knob6;

    knobs[KNOB_7]  = &knob7;
    knobs[KNOB_8]  = &knob8;
    knobs[KNOB_9]  = &knob9;
    knobs[KNOB_10] = &knob10;
    knobs[KNOB_11] = &knob11;
    knobs[KNOB_12] = &knob12;


    for(int i = 0; i < KNOB_LAST; i++)
    {
        knobs[i]->Init(seed.adc.GetPtr(i), seed.AudioCallbackRate());
    }
}

void CsoHardware::InitMidi()
{
    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
}