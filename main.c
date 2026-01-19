#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// This code demonstrates using the C interface to LibPyin from C

#include "source/libpyinc.h"	

#define M_PI acos(-1.0)

int main() {
        int SAMPLE_RATE = 44100;
        int SAMPLE_COUNT = 10000;
        int BLOCK_SIZE = 2048;
        int STEP_SIZE = 512;

        // Prepare objects
        pyinc_init(SAMPLE_RATE, BLOCK_SIZE, STEP_SIZE);
        float* ptr = malloc(sizeof(float) * SAMPLE_COUNT);

        // Generate a 440 herz sine wave
        float freq = 440;
        float angle_speed = 2 * M_PI * (freq / SAMPLE_RATE);
        for (int i = 0; i < SAMPLE_COUNT; i++)
            ptr[i] =  sin(angle_speed * i);

        // Mine pitches
        struct pyinc_pitch_range pitches = pyinc_feed(ptr, SAMPLE_COUNT);

        // Go through and print the pitches (max probability pitch per frame)
        printf("Pitches (max probability per frame): ");
        float* res_ptr = pitches.begin;
        while (res_ptr != pitches.end)
        {
            printf("%f ", *res_ptr);
            res_ptr++;
        }
        printf("\n");
        
        // Get all frequency-probability pairs
        int frame_count = pyinc_get_candidate_frame_count();
        printf("\nFrequency-Probability pairs for each frame:\n");
        for (int frame = 0; frame < frame_count; frame++) {
            struct pyinc_candidate_range candidates = pyinc_get_candidates_for_frame(frame);
            printf("Frame %d: ", frame);
            const struct pyinc_pitch_candidate* cand_ptr = candidates.begin;
            while (cand_ptr != candidates.end) {
                printf("(%f Hz, %f) ", cand_ptr->frequency, cand_ptr->probability);
                cand_ptr++;
            }
            printf("\n");
        }

	free(ptr);

        // Release the data
        pyinc_clear();
}
