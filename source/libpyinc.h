#ifndef PYINC_H
#define PYINC_H

#include "shared_export_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Pointers to the first and one beyond last mined pitch, i.e. the range is [begin, end)
struct pyinc_pitch_range {
    const float * begin;
    const float * end;
};

// Initializes a PYIN object, must be called before using pyinc
// [in]sample_rate  frequency of the track, e.g. 44100 samples per second
// [in]block_size   length of a block used for obtaining a pitch, the higher the slower, 2048 is recommended
// [in]step_size    length of a step between two mined pitches, the smaller the slower, 512 is recommended
void SHARED_EXPORT pyinc_init(const int sample_rate, const int block_size, const int step_size);
// The cut off is a number between [0-1] that says whether the pitch is still to be considered as correct based on the estimate probability
void SHARED_EXPORT pyinc_set_cut_off(const float cut_off);
float SHARED_EXPORT pyinc_get_cut_off();
// Reserves the internal vectors for the given number of expected samples
void SHARED_EXPORT pyinc_reserve(int sample_count);
// Feed new data and obtain the pitches mined using the new data !THE RANGE IS VALID ONLY UNTIL THE NEXT CALL OF pyinc_feed OR pyinc_clear
#ifndef __cplusplus 
struct
#endif
pyinc_pitch_range SHARED_EXPORT pyinc_feed(const float * new_samples, int sample_count);
// Get all the mined pitches
#ifndef __cplusplus 
struct
#endif
pyinc_pitch_range SHARED_EXPORT pyinc_get_pitches();
// Resets to the after-construction state
void SHARED_EXPORT pyinc_clear();

#ifdef __cplusplus
}
#endif


#endif // PYINC_H
