#ifndef CHAT_GLOBAL_H
#define CHAT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CHAT_LIBRARY)
#  define CHAT_EXPORT Q_DECL_EXPORT
#else
#  define CHAT_EXPORT Q_DECL_IMPORT
#endif

#endif // CHAT_GLOBAL_H
