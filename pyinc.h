#ifndef PYINC_H
#define PYINC_H

#include "shared_export_def.h"

extern "C"
{
struct pyinc_pitch_range {
    const float * begin;
    const float * end;
};
void SHARED_EXPORT pyinc_init(const int sample_rate, const int block_size, const int step_size);
//
void SHARED_EXPORT pyinc_delete();
// The cut off is a number between [0-1] that says whether the pitch is still to be considered as correct based on the estimate probability
void SHARED_EXPORT pyinc_set_cut_off(const float cut_off);
float SHARED_EXPORT pyinc_get_cut_off();
// Reserves the internal vectors for the given number of expected samples
void SHARED_EXPORT pyinc_reserve(int sample_count);
// Feed new data and obtain the pitches mined using the new data
pyinc_pitch_range SHARED_EXPORT pyinc_feed(const float * new_samples, int sample_count);
// Get all the mined pitches
pyinc_pitch_range SHARED_EXPORT pyinc_get_pitches();
// Resets to the after-construction state
void SHARED_EXPORT pyinc_clear();
}


#endif // PYINC_H
