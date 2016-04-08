TEMPLATE = lib

CONFIG += c++11

DEFINES += BUILD_SHARED

HEADERS += \
    vamp/vamp.h \
    vamp-sdk/FFT.h \
    vamp-sdk/plugguard.h \
    vamp-sdk/Plugin.h \
    vamp-sdk/PluginAdapter.h \
    vamp-sdk/PluginBase.h \
    vamp-sdk/RealTime.h \
    vamp-sdk/vamp-sdk.h \
    pyin/MeanFilter.h \
    pyin/MonoNote.h \
    pyin/MonoNoteHMM.h \
    pyin/MonoNoteParameters.h \
    pyin/MonoPitch.h \
    pyin/MonoPitchHMM.h \
    pyin/PYIN.h \
    pyin/SparseHMM.h \
    pyin/VampYin.h \
    pyin/Yin.h \
    pyin/YinUtil.h \
    normal_dist_pdf.h \
    libpyincpp.h \
    libpyinc.h \
    shared_export_def.h

SOURCES += \
    vamp-sdk/FFT.cpp \
    vamp-sdk/FFTimpl.cpp \
    vamp-sdk/PluginAdapter.cpp \
    vamp-sdk/RealTime.cpp \
    vamp-sdk/acsymbols.c \
    pyin/libmain.cpp \
    pyin/MonoNote.cpp \
    pyin/MonoNoteHMM.cpp \
    pyin/MonoNoteParameters.cpp \
    pyin/MonoPitch.cpp \
    pyin/MonoPitchHMM.cpp \
    pyin/PYIN.cpp \
    pyin/SparseHMM.cpp \
    pyin/VampYin.cpp \
    pyin/Yin.cpp \
    pyin/YinUtil.cpp \
    normal_dist_pdf.cpp \
    libpyincpp.cpp \
    libpyinc.cpp
