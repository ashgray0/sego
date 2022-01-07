#pragma once
#ifndef DSY_CSO_H
#define DSY_CSO_H
#include <stdint.h>
#include "Utility/dsp.h"
#ifdef __cplusplus

/*
CSO (Cosine Switching Oscillator): Oscillator whose output switches between two phase-locked cosine waves of different frequencies. 
Each cycle begins with the CARRIER wave, switches to the RESONATOR wave (at some point between pi and 2pi), and then switches back
to the carrier wave at the end of the cycle. Modulation of the point at which this switch occurs (controlled by the WIDTH parameter) 
produces a waveform functionally identical to the SAW waveform on the CASIO CZ-101. Modulation of the frequency of the RESONATOR wave
(controlled by the RATIO parameter) can be used in conjuction with WIDTH parameter to recreate the CZ-101's SQUARE waveform and to add
upper harmonics to both the SAW and SQUARE waveforms similar to the CZ-101's various RES waveforms. 

Note that this program does not use the CZ-101's "Phase Distortion" method of synthesis to create these waveforms. Using the CSO method 
of synthesis instead allows us to generate these waveforms in a "mode-less" manner, allowing the user to continuously morph between
waveforms as well as add the RES-style harmonics directly to to the SAW/SQUARE waveforms for a richer sound.

The CSO also includes a FEEDBACK parameter, which allows us to add phase modulation feedback (a la the YAMAHA DX7) to the oscillator
for additional waveshaping. The feedback's phase is inverted (relative to the DX7's implementation) so that the resulting sine --> saw
waveshaping occurs in opposition to the sine --> saw waveshaping of the WIDTH parameter. The combination results in a waveform which
resembles a square wave with clipping/saturation.

The SPREAD parameter adds detune to a set of slave oscillators, similar to the SUPER SAW oscillator mode on the ROLAND JP-8000.
*/

namespace daisysp
{
class Cso
{
  public:
    Cso() {}
    ~Cso() {}

    void Init(float sample_rate)
    {
        sr_        = sample_rate;
        sr_recip_  = 1.0f / sample_rate;
        freq_      = 440.0f;
        ratio_     = 0.0f;
        width_     = 0.5f;
        feedback_  = 0.0f;
        offset_    = 0.0f;
        amp_       = 0.5f;
        phase_     = 0.0f;
        phase_inc_ = CalcPhaseInc(freq_);
        prev_out_  = 0.0f;
        eoc_       = true;
        eor_       = true;
    }

    //FREQ: Fundamental frequency (Hz) of the oscillator
    inline void SetFreq(const float f)
    {
        freq_      = f;
        phase_inc_ = CalcPhaseInc(f);
    }

    //RATIO: Ratio of resonator frequency to carrier/fundamental frequency
    //Normalized s.t. ratio_ += 1 ==> resonator frequncy goes up an octave
    inline void SetRatio(const float r)
    {
        ratio_ = r;
    }

    //WIDTH: Phase value at which the output switches from outputting the carrier to outputting the resonator
    //Normalized s.t. [0:1] ==> [0pi:2pi]
    //Currently set to only go from 0.5 to 1.0
    //Resonator always switches back to carrier at 0pi
    inline void SetWidth(const float w)
    {
        width_ = w;
    }

    //FEEDBACK: Amount of negative phase modulation feedback
    //Same as on the Yamaha DX7 et al.
    inline void SetFeedback(const float fb)
    {
        feedback_ = fb;
    }

    inline void SetOffset(const float o)
    {
        offset_ = o;
    }
    
    
    //AMP: Amplitude of the output
    //Unity is amp_ = 1.0f
    inline void SetAmp(const float a)
    {
        amp_ = a;
    }

    inline bool IsEOR()
    {
        return eor_;
    }

    inline bool IsEOC()
    {
        return eoc_;
        }

    inline bool IsRising()
    {
        return phase_ < PI_F;
    }

    inline bool IsFalling()
    {
        return phase_ >= PI_F;
    }

    float Process();

    void PhaseAdd(float _phase)
    {
        phase_ += (_phase * TWOPI_F);
    }

    void Reset(float _phase = 0.0f)
    {
        phase_ = _phase;
    }

    void CalcPrevOut(float po)
    {
        prev_out_ = po;
    }

  private:
    float   CalcPhaseInc(float f);
    float   amp_, freq_, ratio_, width_, feedback_, offset_;
    float   sr_, sr_recip_, phase_, phase_inc_, prev_out_;
    bool    eor_, eoc_;
};
}
#endif
#endif
