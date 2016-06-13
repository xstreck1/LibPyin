
#include "libpyincpp.h"
#include "pyin/PYIN.h"

#include <algorithm>
#include <cmath>
#include <cassert>
#include <iostream>

PyinCpp::PyinCpp(const int sample_rate, const int block_size, const int step_size) :
    _SAMPLE_RATE(sample_rate),
    _BLOCK_SIZE(block_size),
    _STEP_SIZE(std::min(block_size, step_size)), // Block must be bigger than step
    _TIME_STEP(new Vamp::RealTime(0, static_cast<int>(round(1000000.0 / (sample_rate / step_size))))),
    _pyin(new PYIN(_SAMPLE_RATE))
{
    _cut_off = 0;
    _conversion_head = 0;
    _time = new Vamp::RealTime(0, 0);
    _pyin->initialise(_CHANNEL_COUNT, _STEP_SIZE, _BLOCK_SIZE);
}

void PyinCpp::reserve(const int expected_sample_count) {
    if (expected_sample_count != 0) {
        _samples.reserve(expected_sample_count);
        _pitches.reserve(expected_sample_count / _STEP_SIZE); // It's a few more than needed usually, cause the last block has only the first step tested, no need to optimize
    }
}

PyinCpp::~PyinCpp() {
    delete _pyin;
    delete _time;
    delete _TIME_STEP;
}

void PyinCpp::setCutOff(const float cut_off) {
    _cut_off = cut_off;
}

float PyinCpp::getCutOff() {
    return _cut_off;
}

void PyinCpp::clear() {
    _conversion_head = 0;
    delete _time;
    _time = new Vamp::RealTime(0, 0);
    _pyin->reset();
    _samples.clear();
    _pitches.clear();
}

std::vector<float> PyinCpp::feed(const std::vector<float> & new_samples) {
    // Where we are at now
    size_t last_pitches_count = _pitches.size();

    _samples.insert(std::end(_samples), std::begin(new_samples), std::end(new_samples));

    float* pointers[1];
    // Convert all the available blocks
    while (_samples.size() - _conversion_head > _BLOCK_SIZE) {
        pointers[0] = _samples.data() + _conversion_head;
        PYIN::FeatureSet features = _pyin->process((const float* const*) pointers, *_time);
        *_time = *_time + *_TIME_STEP;
        _conversion_head += _STEP_SIZE;

        // Get the maximal probability pitch
        if (!features.empty() && !features.at(0).empty() && !features.at(0).at(0).values.empty()) {
            auto max_prob_it = std::max_element(begin(features.at(0).at(1).values), end(features.at(0).at(1).values));
            int max_prob_i = std::distance(begin(features.at(0).at(1).values), max_prob_it);
            if (max_prob_i >= _cut_off) {
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

    return vector<float>(std::begin(_pitches) + last_pitches_count, std::end(_pitches));
}

const std::vector<float> &PyinCpp::getPitches() const {
    return _pitches;
}
