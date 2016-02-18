#include "pyincpp.h"

PyinCpp::PyinCpp(const int sample_rate, const int block_size, const int step_size, const float cut_off_threshold, const int expected_sample_count) :
    _SAMPLE_RATE(sample_rate),
    _BLOCK_SIZE(block_size),
    _STEP_SIZE(step_size),
    _TIME_STEP(0, static_cast<int>(std::round(1000000 / (sample_rate / step_size)))),
    _pyin(_SAMPLE_RATE),
    _CUT_OFF_THRESHOLD(cut_off_threshold)
{
    _conversion_head = 0;
    _time = Vamp::RealTime(0, 0);
    _pyin.initialise(_CHANNEL_COUNT, _STEP_SIZE, _BLOCK_SIZE);
    if (expected_sample_count != 0) {
        _samples.reserve(expected_sample_count);
        _pitches.reserve(expected_sample_count / _STEP_SIZE);
    }
}

void PyinCpp::clear() {
    _conversion_head = 0;
    _time = Vamp::RealTime(0, 0);
    _pyin.reset();
    _samples.clear();
    _pitches.clear();
}

void PyinCpp::addPitchesFromFeatures(const PYIN::FeatureSet & features) {
    // Get the maximal probability pitch
    if (!features.empty() && !features.at(0).empty() && !features.at(0).at(0).values.empty()) {
        auto max_prob_it = std::max_element(begin(features.at(0).at(1).values), end(features.at(0).at(1).values));
        int max_prob_i = std::distance(begin(features.at(0).at(1).values), max_prob_it);
        if (max_prob_i >= _CUT_OFF_THRESHOLD) {
            _pitches.emplace_back(features.at(0).at(0).values[max_prob_i]);
        }
        else {
            _pitches.emplace_back(-1);
        }
    }
    else {
        _pitches.emplace_back(-1);
    }
}

std::vector<float> PyinCpp::feed(const std::vector<float> & new_samples) {
    // Where we are at now
    size_t last_pitches_count = _pitches.size();

    _samples.insert(std::end(_samples), std::begin(new_samples), std::end(new_samples));

    float* pointers[1];
    // Convert all the available blocks
    while (_samples.size() - _conversion_head > _BLOCK_SIZE) {
        pointers[0] = _samples.data() + _conversion_head;
        PYIN::FeatureSet features = _pyin.process((const float* const*) pointers, _time);
        _time = _time + _TIME_STEP;
        _conversion_head += _STEP_SIZE;
        addPitchesFromFeatures(features);
    }

    return vector<float>(std::begin(_pitches) + last_pitches_count, std::end(_pitches));
}

const std::vector<float> &PyinCpp::getPitches() const {
    return _pitches;
}
