#include "libpyinc.h"
#include "libpyincpp.h"
#include <memory>

std::unique_ptr<PyinCpp> _pyin_cpp;
std::vector<float> _last_pitches;
std::vector<pyinc_pitch_candidate> _last_candidates;

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
        return _pyin_cpp->getCutOff();
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
    _last_candidates.clear();
}

int pyinc_get_candidate_frame_count() {
    if (_pyin_cpp) {
        return _pyin_cpp->getPitchCandidates().size();
    }
    return 0;
}

pyinc_candidate_range pyinc_get_candidates_for_frame(int frame_index) {
    if (_pyin_cpp) {
        const auto& candidates = _pyin_cpp->getPitchCandidates();
        if (frame_index >= 0 && frame_index < static_cast<int>(candidates.size())) {
            const auto& frame_candidates = candidates[frame_index];
            // Convert to C-compatible structure
            _last_candidates.clear();
            _last_candidates.reserve(frame_candidates.size());
            for (const auto& pair : frame_candidates) {
                _last_candidates.push_back(pyinc_pitch_candidate{pair.first, pair.second});
            }
            return pyinc_candidate_range{_last_candidates.data(), _last_candidates.data() + _last_candidates.size()};
        }
    }
    return pyinc_candidate_range{nullptr, nullptr};
}

