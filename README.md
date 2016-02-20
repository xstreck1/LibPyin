LibPyin pitch detection library
================================

LibPyin is a plug-in library for pitch (fundamental frequency) detection. The library provides simple C and C++ interface for easy incorporability to projects in other languages.

The library depends on the Pyin algorithm by M. Mauch and S. Dixon. See https://code.soundsoftware.ac.uk/projects/pyin for details.

Compiling
---------
A C++11 compliant compiler is needed, however there are no additional dependencies. The repository contains CMake and QMake files for easy compiling. If do not want to use either for some reason, do set the BUILD_SHARED macro to make sure the symbols are correcly exported.

Use
---
The interface is detailed in the header files.
When using from C++, include "libpyincpp.h". When using from C, include "libpyinc.h". 
For other languages the use depends on the language, but in general you will need to call the C interface. 
An example with C# or Unity3D is shown in the file LoadLibrary.cs.

Licence
-------
GNU GPLv3 with attribution, I'm gonna contact the dev's to see if it could be changed to LGPLv3.
The authors of Pyin ask to kindly attribute their work via a citation:
    @inproceedings{matthias2014a,
        author = {Matthias Mauch and Dixon, Simon},
        title = {pYIN: A Fundamental Frequency Estimator Using Probabilistic Threshold Distributions},
        booktitle = {Proceedings of the IEEE International Conference on Acoustics, Speech, and Signal Processing (ICASSP 2014)},
        note = {in press},
        year = {2014}
    } 

