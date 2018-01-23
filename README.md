LibPyin pitch detection library
================================

LibPyin is a plug-in library for pitch (fundamental frequency) detection (see here https://en.wikipedia.org/wiki/Pitch_detection_algorithm). The library provides simple C and C++ interface for easy incorporability to projects in other languages.

The library depends on the Pyin algorithm by M. Mauch and S. Dixon. See https://code.soundsoftware.ac.uk/projects/pyin for details.

Compiling
---------
A C++11 compliant compiler is needed, however there are no additional dependencies. The repository contains [CMake](https://cmake.org/runningcmake/) and [QMake](http://doc.qt.io/qtcreator/creator-project-opening.html) files for easy compiling. 

If do not want to use either for some reason, and are making a project from the code yourself, do set the `LIBPYIN_BUILD_SHARED` macro to make sure the symbols are correcly exported.

Use
---
When using from C++, include `libpyincpp.h`. When using from C, include `libpyinc.h` and link against `LibPyin`.

### Can I use LibPyin Source in My Code?
Absolutelly, just copy-paste the `source` folder to your project and add it to the include path.

Examples
--------
There is C, C++, and C# example present. Each example generates a short sine-wave of 440 hz and extracts the frequency from the wave.
The usage is platform and language dependent. 

For each example first compile the library.

### C example on Unix
    
    gcc main.c -L"." -lLibPyin -lm
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. // Add the current location to the path so the library is loaded
    ./a.out
    
 
### C++ example on Unix
    
    g++ main.cpp -L"." -lLibPyin -std=c++11
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. // Add the current location to the path so the library is loaded
    ./a.out
    
### Unity example

1. Download the Release of the complied .dll .
2. Add LibPyin.dll and LoadLibrary.cs to your Assets.  
3. Call `LoadLibrary.use();` from another script attached to a gameobject.


### C# example on Windows

1. Before building LibPyin, set on the /clr support on the VS Settings/General. (You may need to switch /EHs to /EHa at Code Generation).
2. Build as Debug and Release
3. Run Visual Studio Developer prompt.
3. `csc main.cs LoadLibrary.cs /unsafe /reference:Debug\LibPyin.dll /platform:x86`
4. `main.exe`

Cpp interface
-------------
_Creates a PYIN object, for each track you should create one object  
[in]sample_rate  frequency of the track, e.g. 44100 samples per second  
[in]block_size   length of a block used for obtaining a pitch, the higher the slower, 2048 is recommended  
[in]step_size    length of a step between two mined pitches, the smaller the slower, 512 is recommended_  
    
    PyinCpp(const int sample_rate, const int block_size = _DEFAULT_BLOCK_SIZE, const int step_size = _DEFAULT_STEP_SIZE);
    
_The cut off is a number between [0-1] that says whether the pitch is still to be considered as correct based on the estimate probability (the pitch will be ignored if the probability is lower than the number)_
    
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

    void pyinc_init(const int sample_rate, const int block_size, const int step_size);

_The cut off is a number between [0-1] that says whether the pitch is still to be considered as correct based on the estimate probability (the pitch will be ignored if the probability is lower than the number)_
    
    void SHARED_EXPORT pyinc_set_cut_off(const float cut_off);
    float SHARED_EXPORT pyinc_get_cut_off();

_Reserves the internal vectors for the given number of expected samples_

    void pyinc_reserve(int sample_count);

_Feed new data and obtain the pitches mined using the new data !THE RANGE IS VALID ONLY UNTIL THE NEXT CALL OF pyinc_feed OR pyinc_clear_

    struct pyinc_pitch_range pyinc_feed(const float * new_samples, int sample_count);

_Get all the mined pitches_

    struct pyinc_pitch_range pyinc_get_pitches();

_Resets to the after-construction state_

    void  pyinc_clear();


Licence
-------
GNU GPLv3 with attribution. The authors of Pyin ask to kindly attribute their work via a citation:

```
@inproceedings{matthias2014a,
    author = {Matthias Mauch and Dixon, Simon},
    title = {pYIN: A Fundamental Frequency Estimator Using Probabilistic Threshold Distributions},
    booktitle = {Proceedings of the IEEE International Conference on Acoustics, Speech, and Signal Processing (ICASSP 2014)},
    note = {in press},
    year = {2014}
} 
```

