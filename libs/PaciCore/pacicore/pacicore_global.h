#ifndef PACICORE_GLOBAL_H
#define PACICORE_GLOBAL_H

//#include <QtCore/qglobal.h>

//#if defined(PACICORE_LIBRARY)
//#  define PACICORESHARED_EXPORT Q_DECL_EXPORT
//#else
//#  define PACICORESHARED_EXPORT Q_DECL_IMPORT
//#endif

#ifdef OS_WIN
#ifdef DLL_EXPORT
#define PACICORESHARED_EXPORT __declspec(dllexport)
#else
#define PACICORESHARED_EXPORT __declspec(dllimport)
#endif
#else // OSWIN
#define PACICORESHARED_EXPORT
#endif // OSWIN

#endif // PACICORE_GLOBAL_H
