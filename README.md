LibPyin pitch detection library
================================

LibPyin is a plug-in library for pitch (fundamental frequency) detection (see here https://en.wikipedia.org/wiki/Pitch_detection_algorithm). The library provides simple C and C++ interface for easy incorporability to projects in other languages.

The library depends on the Pyin algorithm by M. Mauch and S. Dixon. See https://code.soundsoftware.ac.uk/projects/pyin for details.

Compiling
---------
A C++11 compliant compiler is needed, however there are no additional dependencies. The repository contains CMake and QMake files for easy compiling. 

If do not want to use either for some reason, do set the BUILD_SHARED macro to make sure the symbols are correcly exported.

Use
---
When using from C++, include "libpyincpp.h". When using from C, include "libpyinc.h". 

For other languages the use depends on the language, but in general you will need to call the C interface. 
An example with C# or Unity3D is shown in the file LoadLibrary.cs.

Cpp interface
-------------
_Creates a PYIN object, for each track you should create one object  
[in]sample_rate  frequency of the track, e.g. 44100 samples per second  
[in]block_size   length of a block used for obtaining a pitch, the higher the slower, 2048 is recommended  
[in]step_size    length of a step between two mined pitches, the smaller the slower, 512 is recommended_  
    
    PyinCpp(const int sample_rate, const int block_size = _DEFAULT_BLOCK_SIZE, const int step_size = _DEFAULT_STEP_SIZE);
    
_The cut off is a number between [0-1] that says whether the pitch is still to be considered as correct based on the estimate probability_
    
    void PyinCpp::setCutOff(const float cut_off);
    float PyinCpp::getCutOff();
    
_Reserves the internal vectors for the given number of expected samples_
    
    void PyinCpp::reserve(int sample_count);
    
_Feed new data and obtain the pitches mined using the new data_

    std::vector<float> PyinCpp::feed(const std::vector<float> & new_samples);
    
_Get all the mined pitches_

    const std::vector<float> & PyinCpp::getPitches() const;

_Resets to the after-construction state_

    void PyinCpp::clear();
	

C interface
-----------
_Initializes a PYIN object, must be called before using pyinc  
[in]sample_rate  frequency of the track, e.g. 44100 samples per second  
[in]block_size   length of a block used for obtaining a pitch, the higher the slower, 2048 is recommended  
[in]step_size    length of a step between two mined pitches, the smaller the slower, 512 is recommended_  

    void SHARED_EXPORT pyinc_init(const int sample_rate, const int block_size, const int step_size);

_The cut off is a number between [0-1] that says whether the pitch is still to be considered as correct based on the estimate probability_
    
    void SHARED_EXPORT pyinc_set_cut_off(const float cut_off);
    float SHARED_EXPORT pyinc_get_cut_off();

_Reserves the internal vectors for the given number of expected samples_

    void SHARED_EXPORT pyinc_reserve(int sample_count);

_Feed new data and obtain the pitches mined using the new data !THE RANGE IS VALID ONLY UNTIL THE NEXT CALL OF pyinc_feed OR pyinc_clear_

    pyinc_pitch_range SHARED_EXPORT pyinc_feed(const float * new_samples, int sample_count);

_Get all the mined pitches_

    pyinc_pitch_range SHARED_EXPORT pyinc_get_pitches();

_Resets to the after-construction state_

    void SHARED_EXPORT pyinc_clear();


Licence
-------
GNU GPLv3 with attribution, I'm gonna contact the dev's to see if it could be changed to LGPLv3.

The authors of Pyin ask to kindly attribute their work via a citation:
```
@inproceedings{matthias2014a,
    author = {Matthias Mauch and Dixon, Simon},
    title = {pYIN: A Fundamental Frequency Estimator Using Probabilistic Threshold Distributions},
    booktitle = {Proceedings of the IEEE International Conference on Acoustics, Speech, and Signal Processing (ICASSP 2014)},
    note = {in press},
    year = {2014}
} 
```

