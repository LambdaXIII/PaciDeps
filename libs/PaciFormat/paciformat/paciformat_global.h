#ifndef PACIFORMAT_GLOBAL_H
#define PACIFORMAT_GLOBAL_H

//#include <QtCore/qglobal.h>

//#if defined(PACIFORMAT_LIBRARY)
//#  define PACIFORMATSHARED_EXPORT Q_DECL_EXPORT
//#else
//#  define PACIFORMATSHARED_EXPORT Q_DECL_IMPORT
//#endif

#ifdef OS_WIN
#ifdef DLL_EXPORT
#define PACIFORMATSHARED_EXPORT __declspec(dllexport)
#else
#define PACIFORMATSHARED_EXPORT __declspec(dllimport)
#endif
#else // OSWIN
#define PACIFORMATSHARED_EXPORT
#endif // OSWIN

#endif // PACIFORMAT_GLOBAL_H
