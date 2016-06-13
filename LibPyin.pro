TEMPLATE = lib

CONFIG += c++11

DEFINES += LIBPYIN_BUILD_SHARED

INCLUDEPATH += "source/"

HEADERS += \
    source/libpyinc.h \
    source/libpyincpp.h \
    source/normal_dist_pdf.h \
    source/shared_export_def.h \
    source/pyin/MeanFilter.h \
    source/pyin/MonoNote.h \
    source/pyin/MonoNoteHMM.h \
    source/pyin/MonoNoteParameters.h \
    source/pyin/MonoPitch.h \
    source/pyin/MonoPitchHMM.h \
    source/pyin/PYIN.h \
    source/pyin/SparseHMM.h \
    source/pyin/VampYin.h \
    source/pyin/Yin.h \
    source/pyin/YinUtil.h \
    source/vamp/vamp.h \
    source/vamp-sdk/FFT.h \
    source/vamp-sdk/plugguard.h \
    source/vamp-sdk/Plugin.h \
    source/vamp-sdk/PluginAdapter.h \
    source/vamp-sdk/PluginBase.h \
    source/vamp-sdk/RealTime.h \
    source/vamp-sdk/vamp-sdk.h

SOURCES += \
    source/libpyinc.cpp \
    source/libpyincpp.cpp \
    source/normal_dist_pdf.cpp \
    source/pyin/libmain.cpp \
    source/pyin/MonoNote.cpp \
    source/pyin/MonoNoteHMM.cpp \
    source/pyin/MonoNoteParameters.cpp \
    source/pyin/MonoPitch.cpp \
    source/pyin/MonoPitchHMM.cpp \
    source/pyin/PYIN.cpp \
    source/pyin/SparseHMM.cpp \
    source/pyin/VampYin.cpp \
    source/pyin/Yin.cpp \
    source/pyin/YinUtil.cpp \
    source/vamp-sdk/FFT.cpp \
    source/vamp-sdk/FFTimpl.cpp \
    source/vamp-sdk/PluginAdapter.cpp \
    source/vamp-sdk/RealTime.cpp

