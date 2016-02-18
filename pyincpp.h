#ifndef PYINCPP_H
#define PYINCPP_H

#include "pyin/PYIN.h"


#ifdef _WIN32
    #ifdef BUILD_SHARED
        #define SHARED_EXPORT __declspec(dllexport)
    #else
        #define SHARED_EXPORT __declspec(dllimport)
    #endif
#else
    #define SHARED_EXPORT
#endif

class SHARED_EXPORT PyinCpp  {
    // as far as I can tell PYIN cannot work with more than 1 channel
    static constexpr int _CHANNEL_COUNT = 1;
    // number of samples per second
    const int _SAMPLE_RATE;
    // length of a step between two mined pitches, the smaller the slower, 512 is recommended
    const int _STEP_SIZE;
    // length of a block used for obtaining a pitch, the higher the slower, 2048 is recommended
    const int _BLOCK_SIZE;
    // the object return only the pitches that have the probability higher than _CUT_OFF_THRESHOLD
    const int _CUT_OFF_THRESHOLD;
    // time difference between two mined pitches
    const Vamp::RealTime _TIME_STEP;

    // PYIN object
    PYIN _pyin;
    // time counter
    Vamp::RealTime _time;
    // Holds all the samples in the current recording (which is of the same length as the song being played)
    std::vector<float> _samples;
    // Holds the extracted fequencies
    std::vector<float> _pitches;
    // First not-yet converted sample
    std::size_t _conversion_head;

    // conversion method, fills the _pitches vector
    void addPitchesFromFeatures(const PYIN::FeatureSet &features);
public:
    PyinCpp(const int sample_rate, const int block_size, const int step_size, const float cut_off_threshold = 0.0, const int expected_sample_count = 0);
    // Feed new data and obtain the pitches mined using the new data
    std::vector<float> feed(const vector<float> & new_samples);
    // Get all the mined pitches
    const std::vector<float> & getPitches() const;
    // Resets to the after-construction state
    void clear();
};

#endif // PYINCPP_H
