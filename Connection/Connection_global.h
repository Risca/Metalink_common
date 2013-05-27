#ifndef CONNECTION_GLOBAL_H
#define CONNECTION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CONNECTION_LIBRARY)
#  define CONNECTIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONNECTIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CONNECTION_GLOBAL_H
