#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// This code demonstrates using the C interface to LibPyin from C

#include "libpyinc.h"	

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

        // Go through and print the pitches
        float* res_ptr = pitches.begin;
        while (res_ptr != pitches.end)
        {
            printf("%f ", *res_ptr);
            res_ptr++;
        }

	free(ptr);

        // Release the data
        pyinc_clear();
}
