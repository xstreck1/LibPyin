#include <iostream>
#include <cmath>
#include <vector>

// This code demonstrates using the C++ interface to LibPyin from C++

#include "source/libpyincpp.h"	

int main() {
    int SAMPLE_RATE = 44100;
    int SAMPLE_COUNT = 10000;
    int BLOCK_SIZE = 2048;
    int STEP_SIZE = 512;

    // Prepare objects
    PyinCpp my_pyin(SAMPLE_RATE);
    my_pyin.setCutOff(0.99);
    std::vector<float> samples(SAMPLE_COUNT);

    // Generate a 440 herz sine wave
    float freq = 440;
    float angle_speed = 2 * M_PI * (freq / SAMPLE_RATE);
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        samples[i] = sin(angle_speed * i);
    }

    // Mine pitches
    std::vector<float> pitches = my_pyin.feed(samples);

    // Go through and print the pitches
    for (const float pitch : pitches)
    {
        std::cout << pitch << " ";
    }
}
