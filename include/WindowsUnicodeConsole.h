/// Patch pour que la console Windows supporte l'affichage en UTF-8.
/// \author Misha Krieger-Raynauld
/// \date 2020-03-04

#ifndef WINDOWSUNICODECONSOLE_H
#define WINDOWSUNICODECONSOLE_H

#ifdef _WIN32
#include <windows.h>
#endif

inline void initializeConsole()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

#endif // WINDOWSUNICODECONSOLE_H
