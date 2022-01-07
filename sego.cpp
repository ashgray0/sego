#include "cso_hardware.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

CsoHardware hw;
Cso         osc1, osc2;
Parameter   p_freq1, p_freq1_ratio, p_width, p_ratio, p_feedback, p_freq2, p_offset, p_cv1;


static float freq1, freq1_ratio, width, ratio, feedback, freq2, offset, cv1;
float        sig1, sig2;
static int   range, mode, scale, shift;

void Controls();

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    Controls();

    for(size_t i = 0; i < size; i += 2)
    {
        sig1      = osc1.Process();
        sig2      = osc2.Process();

        out[i]     = sig1;
        out[i + 1] = sig2;
    }
}

void InitSynth(float samplerate)
{
    p_freq1.Init(hw.knob1, 24.0f, 84.0f, p_freq1.LINEAR);
    p_freq1_ratio.Init(hw.knob1, 1.0f, 8.0f, p_freq1_ratio.LINEAR);
    p_width.Init(hw.knob2, 0.5f, 1.0f, p_width.LINEAR);
    p_ratio.Init(hw.knob3, 0.0f, 7.0f, p_ratio.LINEAR);
    p_feedback.Init(hw.knob4, 0.0f, 2.5f, p_feedback.LINEAR);
    p_freq2.Init(hw.knob5, 0.5f, 2.0f, p_freq2.LINEAR);
    p_offset.Init(hw.knob6, 0.0f, 1.0f, p_offset.LINEAR);

    p_cv1.Init(hw.knob7, 0.5f, -0.5f, p_cv1.LINEAR);
    

    osc1.Init(samplerate);
    osc2.Init(samplerate);

    range = 0;
    mode  = 0;
    scale = 0;
    shift = 0;

}

void UpdateKnobs()
{
    freq1       = p_freq1.Process();
    freq1_ratio = p_freq1_ratio.Process();
    width       = p_width.Process();
    ratio       = p_ratio.Process();
    feedback    = p_feedback.Process();
    freq2       = p_freq2.Process();
    offset      = p_offset.Process();

    cv1         = p_cv1.Process();

    switch(range)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }

    switch(mode)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }

    switch(scale)
    {
        case 0:
            //Quantize: n/a (free-running VCO)
            freq1 = mtof(freq1);
            osc1.SetFreq(freq1);
            osc2.SetFreq(freq1 * freq2);
            break;
        case 1:
            //Quantize: 12TET
            freq1 = mtof(roundf(freq1));
            osc1.SetFreq(freq1);
            osc2.SetFreq(freq1 * freq2);
            break;
        case 2:
            //Quantize: Mult/div of A440 (?)
            freq1 = mtof(24.0f) * roundf(freq1_ratio);
            osc1.SetFreq(freq1);
            osc2.SetFreq(freq1 * freq2);
            break;
        default:
            break;
    }

    switch(shift)
    {
        case 0:
            break;
        case 1:
            break;
        default:
            break;
    }


    //osc1.SetFreq(freq1);
    if(width + cv1 < 0.5f)
    {
        osc1.SetWidth(0.5f);
        osc2.SetWidth(0.5f);
    }
    if(width + cv1 > 1.0f)
    {
        osc1.SetWidth(1.0f);
        osc2.SetWidth(1.0f);
    }
    else
    {
        osc1.SetWidth(width + cv1);
        osc2.SetWidth(width + cv1); 
    }
    
    osc1.SetRatio(ratio);
    osc1.SetFeedback(feedback);

    osc2.SetRatio(ratio);
    osc2.SetFeedback(feedback);
    osc2.SetOffset(offset);
}

void UpdateSwitches()
{
    if(hw.sw1.RisingEdge())
        range += 1;
    range = range % 3;

    if(hw.sw2.RisingEdge())
        mode += 1;
    mode = mode % 4;

    if(hw.sw3.RisingEdge())
        scale += 1;
    scale = scale % 3;

    if(hw.sw4.RisingEdge())
        shift += 1;
    shift = shift % 2;

}

void UpdateLeds() {}
void Controls()

{
    hw.ProcessAnalogControls();
    hw.ProcessDigitalControls();

    UpdateKnobs();
    UpdateSwitches();
    UpdateLeds();
}

int main(void)
{
    float samplerate;

    hw.Init();
    samplerate = hw.AudioSampleRate();
    InitSynth(samplerate);

    hw.StartAdc();
    hw.StartAudio(AudioCallback);

    while(1) {}
}