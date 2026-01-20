#include <iostream>
#include <cmath>
#include <vector>

// This code demonstrates using the C++ interface to LibPyin from C++

#include "source/libpyincpp.h"	

int main() {
    int SAMPLE_RATE = 44100;
    int SAMPLE_COUNT = 2048*2;
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

    // Go through and print the pitches (max probability pitch per frame)
    std::cout << "Pitches (max probability per frame): ";
    for (const float pitch : pitches)
    {
        std::cout << pitch << " ";
    }
    std::cout << std::endl;
    
    // Get all frequency-probability pairs
    const auto& candidates = my_pyin.getPitchCandidates();
    std::cout << "\nFrequency-Probability pairs for each frame:\n";
    for (size_t frame = 0; frame < candidates.size(); ++frame) {
        std::cout << "Frame " << frame << ": ";
        for (const auto& pair : candidates[frame]) {
            std::cout << "(" << pair.first << " Hz, " << pair.second << ") ";
        }
        std::cout << std::endl;
    }
}
