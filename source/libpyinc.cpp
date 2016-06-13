#include "libpyinc.h"
#include "libpyincpp.h"
#include <memory>

std::unique_ptr<PyinCpp> _pyin_cpp;
std::vector<float> _last_pitches;

void pyinc_init(const int sample_rate, const int block_size, const int step_size) {
    _pyin_cpp.reset(new PyinCpp(sample_rate, block_size, step_size));
}


void pyinc_set_cut_off(const float cut_off) {
    if (_pyin_cpp) {
        _pyin_cpp->setCutOff(cut_off);
    }
}

float pyinc_get_cut_off(){
    if (_pyin_cpp) {
        _pyin_cpp->getCutOff();
    }
    else {
        return -1;
    }
}

void  pyinc_reserve(int sample_count){
    if (_pyin_cpp) {
        _pyin_cpp->reserve(sample_count);
    }
}

pyinc_pitch_range pyinc_feed(const float * new_samples, int sample_count){
    if (_pyin_cpp) {
        _last_pitches = _pyin_cpp->feed(std::vector<float>(new_samples, new_samples + sample_count));
        return pyinc_pitch_range{_last_pitches.data(), _last_pitches.data() + _last_pitches.size() };
    }
    else {
        return pyinc_pitch_range{0,0};
    }
}

pyinc_pitch_range pyinc_get_pitches(){
    if (_pyin_cpp) {
        const std::vector<float> & _last_pitches = _pyin_cpp->getPitches();
        return pyinc_pitch_range{_last_pitches.data(), _last_pitches.data() + _last_pitches.size() };
    }
    else {
        return pyinc_pitch_range{0,0};
    }
}

void pyinc_clear(){
    if (_pyin_cpp) {
        _pyin_cpp->clear();
    }
    _last_pitches.clear();
}

