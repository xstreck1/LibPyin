#ifndef PYINCPP_H
#define PYINCPP_H

#include <vector>
#include <memory>

#include "shared_export_def.h"

// Forward declarations of VAMP and PYIN objects
class PYIN;
namespace _VampPlugin{
    namespace Vamp {
        struct RealTime;
    }
}

class SHARED_EXPORT PyinCpp  {
    // as far as I can tell PYIN cannot work with more than 1 channel
    static constexpr int _CHANNEL_COUNT = 1;
    //
    static constexpr int _DEFAULT_BLOCK_SIZE = 2048;
    //
    static constexpr int _DEFAULT_STEP_SIZE = 512;

    // number of samples per second
    const int _SAMPLE_RATE;
    // length of a step between two mined pitches, the smaller the slower, 512 is recommended
    const int _STEP_SIZE;
    // length of a block used for obtaining a pitch, the higher the slower, 2048 is recommended
    const int _BLOCK_SIZE;
    // time difference between two mined pitches
    const _VampPlugin::Vamp::RealTime * _TIME_STEP;

    // PYIN object
    PYIN * _pyin;
    // time counter
    _VampPlugin::Vamp::RealTime * _time;
    // Holds all the samples in the current recording (which is of the same length as the song being played)
    std::vector<float> _samples;
    // Holds the extracted fequencies
    std::vector<float> _pitches;
    // First not-yet converted sample
    std::size_t _conversion_head;

    // Paramteres
    // the thresholds for when to still consider pitch a success
    int _cut_off;

public:
    // Creates a PYIN object, for each track you should create one object
    // [in]sample_rate  frequency of the track, e.g. 44100 samples per second
    // [in]block_size   length of a block used for obtaining a pitch, the higher the slower, 2048 is recommended
    // [in]step_size    length of a step between two mined pitches, the smaller the slower, 512 is recommended
    PyinCpp(const int sample_rate, const int block_size = _DEFAULT_BLOCK_SIZE, const int step_size = _DEFAULT_STEP_SIZE);
    //
    ~PyinCpp();
    // The cut off is a number between [0-1] that says whether the pitch is still to be considered as correct based on the estimate probability
    void setCutOff(const float cut_off);
    float getCutOff();
    // Reserves the internal vectors for the given number of expected samples
    void reserve(int sample_count);
    // Feed new data and obtain the pitches mined using the new data
    std::vector<float> feed(const std::vector<float> & new_samples);
    // Get all the mined pitches
    const std::vector<float> & getPitches() const;
    // Resets to the after-construction state
    void clear();
};

#endif // PYINCPP_H
