#ifndef PACIFORTUNE_GLOBAL_H
#define PACIFORTUNE_GLOBAL_H

#ifdef OS_WIN
#ifdef DLL_EXPORT
#define PACIFORTUNESHARED_EXPORT __declspec(dllexport)
#else
#define PACIFORTUNESHARED_EXPORT __declspec(dllimport)
#endif
#else // OSWIN
#define PACIFORTUNESHARED_EXPORT
#endif // OSWIN

#endif // PACIFORTUNE_GLOBAL_H
