#include "dsp.h"
#include "cso.h"

using namespace daisysp;

float Cso::Process()
{
    float out;

    if(phase_ < width_ * TWOPI_F)
    {
        //CARRIER
        out = cosf(offset_ * TWOPI_F) * (0.5f * cosf(ratio_ * TWOPI_F) + 0.5f) * -1.0f * cosf(0.5f * phase_ / width_ + (feedback_ * prev_out_)) + cosf(offset_ * TWOPI_F) * (-0.5f * cos(ratio_ * TWOPI_F) + 0.5f);
        //out = cosf(offset_ * TWOPI_F) * (0.5f * cosf(ratio_ * TWOPI_F) + 0.5f) * cosf(0.5f * phase_ / width_ - feedback_ * prev_out_) - (-0.5f * cos(ratio_ * TWOPI_F) + 0.5f);
        //out = 0.5f * (cosf(offset_ * TWOPI_F) - cosf((ratio_ + offset_) * TWOPI_F)) * cosf(0.5f * phase_ / width_ - feedback_ * prev_out_) + 0.5f * (cos(ratio_ * TWOPI_F) + cos((ratio_ + offset_) * TWOPI_F));
        //out = cosf((offset_ + ratio_) * TWOPI_F) * cosf(0.5f * phase_ / width_ - feedback_ * prev_out_);
    }

    else
    {
        //RESONATOR
        out = cosf(0.5f * (2.0f * ratio_ + 1.0f) * (phase_ - width_ * TWOPI_F) / (1.0f - width_) + (feedback_ * prev_out_) + offset_ * TWOPI_F);
    } 

    phase_ += phase_inc_;

    if(phase_ > TWOPI_F)
    {
        phase_ -= TWOPI_F;
        eoc_ = true;
    }

    else
    {
        eoc_ = false;
    }

    eor_ = (phase_ - phase_inc_ < PI_F && phase_ >= PI_F);

    Cso::CalcPrevOut(out);

    return out * amp_;
}

float Cso::CalcPhaseInc(float f)
{
    return (TWOPI_F * f) * sr_recip_;
}