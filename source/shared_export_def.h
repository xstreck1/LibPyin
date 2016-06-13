#ifndef SHARED_EXPORT_DEF_H
#define SHARED_EXPORT_DEF_H

#ifdef _WIN32
#ifdef LIBPYIN_BUILD_SHARED
#define SHARED_EXPORT __declspec(dllexport)
#else
#define SHARED_EXPORT __declspec(dllimport)
#endif
#else
#define SHARED_EXPORT
#endif

#endif // SHARED_EXPORT_DEF_H
